#include "dfa.h"

void
edges(state_t* state, int symbol, bitset_t* result) {
	if (state->trans) {
		edge_t* it = state->trans;
		while (it) {
			if (it->label == symbol)
				{ ADD_BITSET(result, it->out_state->index_state); }
			it = it->next;
		}
	}
	if (state->class && IS_PRESENT(state->class, (unsigned)symbol))
		{ ADD_BITSET(result, state->out_class->index_state); }
}

bitset_t*
closure(bitset_t* set_state) {
	if (!set_state)
		{ return (NULL); }
	bitset_t* last = NULL_BITSET;
	do {
		del_bitset(last);
		last = dup_bitset(set_state);
		int i;
		while ((i = IT_NEXT(last)) != -1)
			{ edges(STATE_AT(i), EPSILON, set_state); }
	} while (!eq_bitset(set_state, last));
	del_bitset(last);
	return (set_state);
}

bitset_t*
DFAedge(bitset_t* states, int symbol) {
	bitset_t* target = new_bitset();
	int i;
	while ((i = IT_NEXT(states)) != -1)
		{ edges(STATE_AT(i), symbol, target); }
	IT_RESET(states);
	if (is_empty_bitset(target)) {
		del_bitset(target);
		return (NULL_BITSET);
	}
	return (closure(target));
}

static vector_t* gen_state_table(state_t* master, vector_t** rstates) {
	vector_t* states = (*rstates) = new_vector();
	
	bitset_t* start = new_bitset();
	ADD_BITSET(start, master->index_state);
	
	PUSH_BACK_VECTOR(states, NULL_BITSET);
	PUSH_BACK_VECTOR(states, closure(start));
	
	//vector_t* trans = new_vector();
	//PUSH_BACK_VECTOR(trans, NULL);	//Dead State

	long j = 1, p = 1;

	while (j <= p) {
		//printf("J = %ld\n", j);
		//PUSH_BACK_VECTOR(trans, NULL);
		for (register int i = MIN_ASCII; i < MAX_ASCII; ++i) {
			bitset_t* next = DFAedge(AT_VECTOR(states, j), i);
			if (next == NULL_BITSET)
				{ continue; }

			//trans_list_t* new_list = NEW(trans_list_t, 1);
			//new_list->input = i;

			long l = 1;
			for (; l <= p; ++l) {
				if (eq_bitset(next, (bitset_t*)AT_VECTOR(states, l)))
					{ break; }
			}
			if (l <= p) {
				//new_list->state = l;
				del_bitset(next);
			}
			else {
				++p;
				//new_list->state = ++p;
				PUSH_BACK_VECTOR(states, next);
			}
			//new_list->next = AT_VECTOR(trans, j);
			//SET_VECTOR(trans, j, new_list);
		}
		++j;
	}
	exit(1);
	return (NULL);
	//return (trans);
}

static vector_t* gen_final_table(vector_t* states, vector_t* elst) {
	vector_t* final = new_vector();
	for (size_t i = 1; i < SIZE_VECTOR(states); ++i) {
		bitset_t* set_state = (bitset_t*)AT_VECTOR(states, i);
		int min_tok = 0;
		int it;
		while ((it = IT_NEXT(set_state)) != -1) {
			state_t* crt_state = STATE_AT(it);
			if (crt_state->final) {
				if (!min_tok || min_tok > crt_state->final)
					{ min_tok = crt_state->final; }
			}
		}
		if (min_tok) {
			char* name = ((token_entry_t*)
					AT_VECTOR(elst, min_tok - 1))->name;
			PUSH_BACK_VECTOR(final, (void*)i);
			PUSH_BACK_VECTOR(final, name);
		}
	}
	return (final);
}

static void gen_table(state_t* master, vector_t** trans,
					vector_t** final, vector_t* elst) {
	vector_t* states = NULL;
	*trans = gen_state_table(master, &states);
	*final = gen_final_table(states, elst);
	for (size_t i = 0; i < SIZE_VECTOR(states); ++i)
		{ del_bitset((bitset_t*)AT_VECTOR(states, i)); }
	del_vector(states);
}

#ifdef OPTIMIZE
static void
redirect_trans(vector_t* trans, long s1, long s2) {
	del_vector(at_vector(trans, s2));
	erase_vector(trans, s2);
	for (size_t i = 1; i < SIZE_VECTOR(trans); ++i) {
		vector_t* row = (vector_t*)AT_VECTOR(trans, i);
		for (size_t j = 0; j < SIZE_VECTOR(row); ++j) {
			if ((long)AT_VECTOR(row, j) == s2)
				{ SET_VECTOR(row, j, (void*)s1); }
			else if ((long)AT_VECTOR(row, j) > s2) {
				long old = (long)AT_VECTOR(row, j) - 1;
				SET_VECTOR(row, j, (void*)old);
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

	for (size_t i = 0; i < SIZE_VECTOR(finalt); i += 2) {
		if ((size_t)AT_VECTOR(finalt, i) > max) {
			long old = (long)at_vector(finalt, i) - 1;
			SET_VECTOR(finalt, i, (void*)old);
		}
	}
}

static void 
equivalent_state(vector_t* trans, vector_t* finalt) {
	bool change = true;
	while (change) {
		change = false;
		for (size_t i = 1; i < SIZE_VECTOR(trans); ++i) {
			vector_t* s1 = (vector_t*)AT_VECTOR(trans, i);
			for (size_t j = i + 1; j < SIZE_VECTOR(trans); ++j) {
				vector_t* s2 = (vector_t*)AT_VECTOR(trans, j);
				bool equiv = true;
				for (int k = MIN_ASCII; k < MAX_ASCII; ++k) {
					if (AT_VECTOR(s1, k) != AT_VECTOR(s2, k)) {
						equiv = false;
						break;
					}
				}
		
				if (!equiv)
					{ continue; }
			
				int fs1 = get_index_vector(finalt, (void*)i);
				int fs2 = get_index_vector(finalt, (void*)j);

				bool final = (fs1 >= 0 && fs2 >= 0) &&
					(AT_VECTOR(finalt, fs1 + 1)
					== AT_VECTOR(finalt, fs2 + 1));
			
				bool nonfinal = (fs1 == -1 && fs2 == -1);
		
				if (final || nonfinal) {
					change = true;
					redirect_trans(trans, i, j);
					redirect_final(finalt, final, fs2, j);
				}
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
	(void)base;
#if 0
#ifdef OPTIMIZE
	equivalent_state(trans, final);
#endif /* OPTIMIZE */
	
	char const* header_filename = strjoin(base, ".h");
	out_header(header_filename, trans, final, spec->entry_lst);
	FREE(header_filename);
#endif
	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i)
		{ del_vector(AT_VECTOR(trans, i)); }
	del_vector(trans);
	del_vector(final);
	del_token_spec(spec);
	del_record();
}

