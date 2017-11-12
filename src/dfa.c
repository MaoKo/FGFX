#include "dfa.h"

set_t* edges(state_t* st, int symbol) {
	if (!st)
		{ return (NULL); }
	set_t* out_states = EMPTY_SET;
	set_t* it = st->edges;
	while (it) {
		edge_t* edg = (edge_t*)it->item;
		if (edg->symbol == symbol)
			{ add_set(&out_states, edg->out_state); }
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
			set_t* pass_state = edges((state_t*)it->item, EPSILON);
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
		set_t* pass_state = edges((state_t*)it->item, symbol);
		union_set(&target, pass_state);
		del_set(pass_state);
		it = it->next;
	}
	return (closure(&target));
}

static void out_require_header(void) {
	puts("#include <stdint.h>\n");
}

static void out_get_tok_id(void) {
	puts(STATIC" "INT" "FUNC(get_tok_from_state, int state)" "BEG_BLOCK);
	puts(TAB FOR(int i = 0, i < SIZE_FINAL_TAB, ++i)" "BEG_BLOCK);
	puts(TAB TAB IF(final_table[i][0] == state));
	puts(TAB TAB TAB BEG_BLOCK" "RETURN(final_table[i][1])""SEMI" "END_BLOCK);
	puts(TAB END_BLOCK);
	puts(TAB RETURN(TNONE)""SEMI);
	puts(END_BLOCK);
}

static void out_enum(vector_t* elst) {
	puts(ENUM" "BEG_BLOCK);
	puts(TAB"TNONE,");
	for (size_t i = 0; i < size_vector(elst); ++i) {
		token_entry_t* te = (token_entry_t*)at_vector(elst, i);
		if (!te->local)
			{ printf(TAB"T%s,\n", te->name); }
	}
	puts(END_BLOCK""SEMI"\n");
}

static vector_t* gen_state_table(state_t* master, vector_t** states) {
	(*states) = vector();
	vector_t* hash_state = vector();
	
	set_t* start = set(master);

	push_back_vector(*states, EMPTY_SET);
	push_back_vector(*states, closure(&start));

	push_back_vector(hash_state, (void*)hash_set(NULL));
	push_back_vector(hash_state, (void*)hash_set(start));

	vector_t* trans = vector();
	long j = 0, p = 1;

	while (j <= p) {
		push_back_vector(trans, vector());
		vector_t* row = (vector_t*)back_vector(trans);
		for (unsigned char i = MIN_ASCII; i < MAX_ASCII; ++i) {
			set_t* next = DFAedge(at_vector(*states, j), i);
			if (next == EMPTY_SET) {
				push_back_vector(row, (void*)0L);	
				continue;
			}
			long l = 1;
			size_t hash_next = hash_set(next);
			for (; l <= p; ++l) {
				if (hash_next == (size_t)at_vector(hash_state, l) &&
					equal_set(next, at_vector(*states, l)))
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
			state_t* crt_state = (state_t*)(it->item);
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

static void gen_table(state_t* master, vector_t** trans,
					vector_t** final, vector_t* elst) {
	vector_t* states = NULL;
	*trans = gen_state_table(master, &states);
	*final = gen_final_table(states, elst);
	for (size_t i = 0; i < size_vector(states); ++i)
		{ del_set((set_t*)at_vector(states, i)); }
	del_vector(states);
}

static size_t min_size_type(size_t size) {
	if (size <= UCHAR_MAX) { return (8); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (16); }
	return (32);
}

static void out_state_table(vector_t* trans) {
	printf("//Size of state table = %zu\n", size_vector(trans));
	printf("static uint%zu_t state_table[][%d] = {\n",
			min_size_type(size_vector(trans)), CHAR_MAX);

	for (size_t i = 0; i < size_vector(trans); ++i) {
		vector_t* state = (vector_t*)at_vector(trans, i);
		printf(TAB BEG_BLOCK);
		for (size_t j = 0; j < size_vector(state); ++j) {
			long out = (long)at_vector(state, j);
			if (out)
				{ printf("[%zu]=%ld, ", j, out); }
		}
		puts(END_BLOCK",");
	}
	puts(END_BLOCK""SEMI"\n");
}

static void out_final_table(vector_t* final) {
	printf("static uint%zu_t final_table[][2] = {\n",
			min_size_type(size_vector(final)));
	size_t count_final = size_vector(final) / 2;
	for (size_t i = 0; i < count_final; ++i) {
		printf(TAB"{%ld, "TAB"T%s},\n", (long)at_vector(final, i*2),
				(char const*)at_vector(final, i*2+1));
	}
	printf("};\n\n#define SIZE_FINAL_TAB %zu\n\n", count_final);
}

static void out_useful_macro(void) {
	puts("#define START_STATE"TAB"1");
	puts("#define DEAD_STATE"TAB"0\n");
}

#ifdef OPTIMIZE
static void redirect_trans(vector_t* trans, long s1, long s2) {
	del_vector(at_vector(trans, s2));
	erase_vector(trans, s2);
	for (size_t i = 0; i < size_vector(trans); ++i) {
		vector_t* row = (vector_t*)at_vector(trans, i);
		for (size_t j = 0; j < size_vector(row); ++j) {
			if ((long)at_vector(row, j) == s2)
				{ set_vector(row, j, (void*)s1); }
			else if ((long)at_vector(row, j) > s2) {
				long old = (long)at_vector(row, j) - 1;
				set_vector(row, j, (void*)old);
			}
		}
	}
}

static void redirect_final(vector_t* finalt, bool isf, size_t fs2, size_t max) {
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

static void equivalent_state(vector_t* trans, vector_t* finalt) {
	puts("//Debug optimization DFA (equivalent state)");
	for (size_t i = 0; i < size_vector(trans); ++i) {
		vector_t* s1 = (vector_t*)at_vector(trans, i);
		for (size_t j = i + 1; j < size_vector(trans); ++j) {
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

			int fs1 = get_index_vector(finalt, (void*)i);
			int fs2 = get_index_vector(finalt, (void*)j);

			bool final = (fs1 >= 0 && fs2 >= 0) &&
				(at_vector(finalt, fs1 + 1) ==
				 at_vector(finalt, fs2 + 1));
			
			bool nonfinal = (fs1 == -1 && fs2 == -1);
			
			if (final || nonfinal) {
				printf("//State %zu - %zu\n", i, j);
				redirect_trans(trans, i, j);
				redirect_final(finalt, final, fs2, j);
			}
		}
	}
}
#endif /* OPTIMIZE */

void DFAgen(token_spec_t* spec) {
	if (!spec)
		{ return; }
	vector_t* trans = NULL;
	vector_t* final = NULL;
	gen_table(spec->master, &trans, &final, spec->entry_lst);

#ifdef OPTIMIZE
	equivalent_state(trans, final);
#endif /* OPTIMIZE */

	out_require_header();
	out_useful_macro();
	out_enum(spec->entry_lst);
	out_state_table(trans);
	out_final_table(final);
	out_get_tok_id();

	for (size_t i = 0; i < size_vector(trans); ++i)
		{ del_vector(at_vector(trans, i)); }
	del_vector(trans);
	del_vector(final);
}

