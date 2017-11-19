#include "dfa.h"

set_t* edges(long st, int symbol) {
	set_t* out_states = EMPTY_SET;
	set_t* it = state_at(st)->edges;
	while (it) {
		edge_t* edg = (edge_t*)it->item;
		if (edg->symbol == symbol)
			{ add_set(&out_states, (void*)edg->out_state); }
		it = it->next;
	}
	return (out_states);
}

set_t* closure(set_t** set_state) {
	if (!set_state)
		{ return (EMPTY_SET); }
	set_t* last = EMPTY_SET;
	do {
		del_set(last);
		last = cpy_set(*set_state);
		set_t* it = last;
		while (it) {
			set_t* pass_state = edges((long)it->item, EPSILON);
			union_set(set_state, pass_state);
			del_set(pass_state);
			it = it->next;
		}
	} while (!equal_set(*set_state, last));
	del_set(last);
	return (*set_state);
}

set_t* DFAedge(set_t* states, int symbol) {
	set_t* target = EMPTY_SET;
	set_t* it = states;
	while (it) {
		set_t* pass_state = edges((long)it->item, symbol);
		union_set(&target, pass_state);
		del_set(pass_state);
		it = it->next;
	}
	return (closure(&target));
}

static vector_t* gen_state_table(long master, vector_t** states) {
	(*states) = vector();
	vector_t* hash_state = vector();

	set_t* start = set((void*)master);
	
	push_back_vector(*states, EMPTY_SET);
	push_back_vector(*states, closure(&start));

	print_set(start);
	
	push_back_vector(hash_state, (void*)hash_set(NULL));
	push_back_vector(hash_state, (void*)hash_set(start));

	vector_t* trans = vector();
	long j = 0, p = 1;

	while (j <= p) {
		push_back_vector(trans, vector());
		vector_t* row = (vector_t*)back_vector(trans);
		for (int i = MIN_ASCII; i < MAX_ASCII; ++i) {
			set_t* next = DFAedge(at_vector(*states, j), i);
			if (next == EMPTY_SET) {
				push_back_vector(row, (void*)0L);	
				continue;
			}
			long l = 1;
			size_t hash_next = hash_set(next);
			print_set(next);
			for (; l <= p; ++l) {
				if (hash_next == (size_t)at_vector(hash_state, l)
					&& equal_set(next, at_vector(*states, l)))
					{ break; }
			}
			if (l <= p) {
				push_back_vector(row, (void*)l);
				del_set(next);
			}
			else {
				push_back_vector(row, (void*)++p);
				push_back_vector(*states, next);
				push_back_vector(hash_state, (void*)hash_next);
			}
		}
		++j;
	}

	del_vector(hash_state);
	return (trans);
}

static vector_t* gen_final_table(vector_t* states, vector_t* elst) {
	vector_t* final = vector();
	for (size_t i = 1; i < size_vector(states); ++i) {
		set_t* set_state = (set_t*)at_vector(states, i);
		int min_tok = 0;
		set_t* it = set_state;
		while (it) {
			state_t* crt_state = state_at((long)(it->item));
			if (crt_state->final) {
				if (!min_tok || min_tok > crt_state->final)
					{ min_tok = crt_state->final; }
			}
			it = it->next;
		}
		if (min_tok) {
			char* name = ((token_entry_t*)
					at_vector(elst, min_tok - 1))->name;
			push_back_vector(final, (void*)i);
			push_back_vector(final, name);
		}
	}
	return (final);
}

static void gen_table(int master, vector_t** trans,
					vector_t** final, vector_t* elst) {
	vector_t* states = NULL;
	*trans = gen_state_table(master, &states);
	*final = gen_final_table(states, elst);
	for (size_t i = 0; i < size_vector(states); ++i)
		{ del_set((set_t*)at_vector(states, i)); }
	del_vector(states);
}

#ifdef OPTIMIZE
static void
redirect_trans(vector_t* trans, long s1, long s2) {
	del_vector(at_vector(trans, s2));
	erase_vector(trans, s2);
	for (size_t i = 1; i < size_vector(trans); ++i) {
		vector_t* row = (vector_t*)at_vector(trans, i);
		for (size_t j = 0; j < size_vector(row); ++j) {
			if ((long)at_vector(row, j) == s1)
				{ set_vector(row, j, (void*)s2); }
			else if ((long)at_vector(row, j) > s1) {
				long old = (long)at_vector(row, j) - 1;
				set_vector(row, j, (void*)old);
			}
		}
	}
}

static void
redirect_final(vector_t* finalt, bool isf, size_t fs2, size_t max) {
	if (isf) {
		erase_vector(finalt, fs2);
		erase_vector(finalt, fs2);
	}

	for (size_t i = 0; i < size_vector(finalt); i += 2) {
		if ((size_t)at_vector(finalt, i) > max) {
			long old = (long)at_vector(finalt, i) - 1;
			set_vector(finalt, i, (void*)old);
		}
	}
}

static void 
equivalent_state(vector_t* trans, vector_t* finalt) {
	for (size_t i = size_vector(trans) - 1; i > 1; --i) {
		vector_t* s1 = (vector_t*)at_vector(trans, i);
		for (size_t j = i - 1; j > 1; --j) {
			vector_t* s2 = (vector_t*)at_vector(trans, j);
			bool equiv = true;
			for (int k = MIN_ASCII; k < MAX_ASCII; ++k) {
				if (at_vector(s1, k) != at_vector(s2, k)) {
					equiv = false;
					break;
				}
			}
		
			if (!equiv)
				{ continue; }
			
			//printf("I, J = %zu, %zu\n", i, j);

			int fs1 = get_index_vector(finalt, (void*)i);
			int fs2 = get_index_vector(finalt, (void*)j);

			bool final = (fs1 >= 0 && fs2 >= 0) &&
				(at_vector(finalt, fs1 + 1)
				== at_vector(finalt, fs2 + 1));
			
			bool nonfinal = (fs1 == -1 && fs2 == -1);
		
			if (final || nonfinal) {
				redirect_trans(trans, i, j);
				redirect_final(finalt, final, fs2, j);
				--i;
			}
		}
	}
}
#endif /* OPTIMIZE */

void DFAgen(token_spec_t* spec, char const* base) {
	if (!spec)
		{ return; }
	vector_t* trans = NULL;
	vector_t* final = NULL;
	gen_table(spec->master, &trans, &final, spec->entry_lst);

#ifdef OPTIMIZE
	equivalent_state(trans, final);
#endif /* OPTIMIZE */

	char const* header_filename = strjoin(base, ".h");
	char const* body_filename = strjoin(base, ".c");

	out_header(header_filename, spec->entry_lst);
	out_body(body_filename, header_filename, trans, final);

	FREE(header_filename);
	FREE(body_filename);

	for (size_t i = 0; i < size_vector(trans); ++i)
		{ del_vector(at_vector(trans, i)); }
	del_vector(trans);
	del_vector(final);
	del_record();
}

