#include "dfa.h"
#include "output_dfa.h"
#include "token_def.h"
#include "nfa.h"
#include "utils.h"

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
	if (state->class && (symbol != EPSILON &&
				IS_PRESENT(state->class, (unsigned)symbol)))
		{ ADD_BITSET(result, state->out_class->index_state); }
}

bitset_t*
epsilon_closure(bitset_t* set_state) {
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
	return (epsilon_closure(target));
}

static vector_t*
gen_state_table(state_t* master, vector_t** rstates) {
	vector_t* states = (*rstates) = new_vector();
	
	bitset_t* start = new_bitset();
	ADD_BITSET(start, master->index_state);
	
	PUSH_BACK_VECTOR(states, NULL_BITSET);
	PUSH_BACK_VECTOR(states, epsilon_closure(start));
	
	vector_t* trans = new_vector();
	PUSH_BACK_VECTOR(trans, NULL_TRANS_LST); //Dead State

	long j = 1, p = 1;

	while (j <= p) {
		//printf("J = %ld\n", j);
		PUSH_BACK_VECTOR(trans, NULL_TRANS_LST);
		for (register int i = MIN_ASCII; i < MAX_ASCII; ++i) {
			bitset_t* next = DFAedge(AT_VECTOR(states, j), i);
			if (next == NULL_BITSET)
				{ continue; }

			trans_list_t* new_list = NEW(trans_list_t, 1);
			new_list->input = i;

			long l = 1;
			for (; l <= p; ++l) {
				if (eq_bitset(next, (bitset_t*)AT_VECTOR(states, l)))
					{ break; }
			}
			if (l <= p) {
				new_list->state = l;
				del_bitset(next);
			}
			else {
				new_list->state = ++p;
				PUSH_BACK_VECTOR(states, next);
			}
			new_list->next = AT_VECTOR(trans, j);
			SET_VECTOR(trans, j, new_list);
		}
		++j;
	}
	return (trans);
}

static vector_t*
gen_final_table(vector_t* states, vector_t* elst) {
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

static void
gen_table(state_t* master, vector_t** trans, vector_t** final, vector_t* elst) {
	vector_t* states = NULL;
	*trans = gen_state_table(master, &states);
	*final = gen_final_table(states, elst);
	for (size_t i = 0; i < SIZE_VECTOR(states); ++i)
		{ del_bitset((bitset_t*)AT_VECTOR(states, i)); }
	del_vector(states);
	del_record();
}

#ifdef OPTIMIZE
static void
redirect_trans(vector_t* trans, long s1, long s2) {
	del_trans_list(AT_VECTOR(trans, s2));
	erase_vector(trans, s2);
	for (size_t i = 1; i < SIZE_VECTOR(trans); ++i) {
		trans_list_t* t = (trans_list_t*)AT_VECTOR(trans, i);
		while (t) {
			if (t->state == s2)
				{ t->state = s1; }
			else if (t->state > s2)
				{ --(t->state); }
			t = t->next;
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
			long old = (long)AT_VECTOR(finalt, i) - 1;
			SET_VECTOR(finalt, i, (void*)old);
		}
	}
}

static void 
equivalent_state(vector_t* trans, vector_t* finalt) {
	bool repeat;
	do {
		repeat = false;
		for (size_t i = 1; i < SIZE_VECTOR(trans); ++i) {
			trans_list_t const* t1 = (trans_list_t*)AT_VECTOR(trans, i);
			for (size_t j = i + 1; j < SIZE_VECTOR(trans); ++j) {
				trans_list_t const* t2 = (trans_list_t*)
								AT_VECTOR(trans, j);
				if (!cmp_trans_list(t1, t2))
					{ continue; }
			
				int fs1 = get_index_vector(finalt, (void*)i, NULL);
				int fs2 = get_index_vector(finalt, (void*)j, NULL);

				bool final = (fs1 >= 0 && fs2 >= 0) &&
					(AT_VECTOR(finalt, fs1 + 1)
					== AT_VECTOR(finalt, fs2 + 1));
			
				bool nonfinal = (fs1 == -1 && fs2 == -1);
		
				if (final || nonfinal) {
					repeat = true;
					redirect_trans(trans, i, j);
					redirect_final(finalt, final, fs2, j);
				}
			}
		}
	} while (repeat);
}
#endif /* OPTIMIZE */

void
dfa_gen(token_spec_t* spec, char const* base_file) {
	if (!spec)
		{ return; }
	vector_t* trans = NULL;
	vector_t* final = NULL;
	gen_table(spec->master, &trans, &final, spec->entry_lst);

#ifdef OPTIMIZE
	equivalent_state(trans, final);
#endif /* OPTIMIZE */

	output_dfa_matrix(base_file, trans, final, spec->entry_lst);
	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i)
		{ del_trans_list(AT_VECTOR(trans, i)); }

	del_vector(trans); del_vector(final);
	del_token_spec(spec);
}

