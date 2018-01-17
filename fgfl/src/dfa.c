#include "dfa.h"
#include "gen_dfa.h"
#include "lexical_spec.h"
#include "nfa.h"
#include "utils.h"

static void
nfa_automaton_edge(nfa_automaton_t* nfa_m, int symbol, bitset_t* result) {
    switch (nfa_m->kind_nfa) {
        case NFA_UNION:
        case NFA_CLASS:
            if (symbol == EPSILON)
                { ADD_BITSET(result, GET_INDEX(nfa_m->tail_state)); }
            break;

        case NFA_SYMBOL:
            if (nfa_m->out_symbol == symbol)
                { ADD_BITSET(result, GET_INDEX(nfa_m->head_state)); }
            break;

        case NFA_CLOSURE:
        case NFA_EPSILON:
            if (symbol == EPSILON)
                { ADD_BITSET(result, GET_INDEX(nfa_m->head_state)); }
            break;

        case NFA_CONCAT:
            nfa_automaton_edge(nfa_m->left, symbol, result);
            break;

        case NFA_MASTER: ;
                int i;
                while ((i = IT_NEXT(nfa_m->out_automata)) != IT_NULL) {
                    nfa_automaton_t* crt_nfa_m = AUTOMATON_AT(i);
                    nfa_automaton_edge(crt_nfa_m, symbol, result);
                }
                IT_RESET(nfa_m->out_automata);
                break;

        default:
            break;
    }
}

static void
nfa_state_edges(nfa_state_t* state, int symbol, bitset_t* result) {
    switch (state->symbol_edge) {
        case EDGE_AUTOMATA:
            nfa_automaton_edge(state->edge, symbol, result);
            if (state->edge2)
                { nfa_automaton_edge(state->edge2, symbol, result); }
            break;

        case EDGE_CLASS:
            if ((symbol != EPSILON)
                                && (IS_PRESENT(state->class, (size_t)symbol)))
                { ADD_BITSET(result, GET_INDEX(state->out_state)); }
            break;

        case NO_EDGE:
            break;

        default:
            if (symbol == state->symbol_edge)
                { ADD_BITSET(result, GET_INDEX(state->out_state)); }
    }    
}

static bitset_t*
epsilon_closure(bitset_t* set_state) {
	if (!set_state)
		{ return (NULL_BITSET); }

	bitset_t* seen_state = new_bitset();
	bool change = false;

	do {
		int i;
		change = false;

		while ((i = IT_NEXT(set_state)) != IT_NULL) {
			if (!IS_PRESENT(seen_state, (size_t)i)) {
                nfa_state_t* crt_state = STATE_AT(i);
                if (crt_state->eclos) {
                    if (!is_subset_bitset(set_state, crt_state->eclos))
                        { UNION_BITSET(set_state, crt_state->eclos); }
                    else
                        { continue; }
                }
                else {
    				nfa_state_edges(STATE_AT(i), EPSILON, set_state);
	    			ADD_BITSET(seen_state, (size_t)i);
                }
                change = true;
			}
		}
		IT_RESET(set_state);
	} while (change);

	del_bitset(seen_state);
	return (set_state);
}

static bitset_t*
dfa_edge(bitset_t* states, int symbol) {
	bitset_t* target = new_bitset();

	int i;
	while ((i = IT_NEXT(states)) != IT_NULL)
		{ nfa_state_edges(STATE_AT(i), symbol, target); }
	IT_RESET(states);

	if (is_empty_bitset(target)) {
		del_bitset(target);
		return (NULL_BITSET);
	}

	return (epsilon_closure(target));
}

static vector_t*
build_state_table(nfa_state_t* master, vector_t** return_states) {
	vector_t* states = (*return_states) = new_vector();
	
	PUSH_BACK_VECTOR(states, NULL_BITSET);
	PUSH_BACK_VECTOR(states, master->eclos);
	
	vector_t* trans = new_vector();
	PUSH_BACK_VECTOR(trans, NULL_TRANS_LST); //Dead State

	long j = 1;
	long p = 1;

	while (j <= p) {
		//printf("J = %ld\n", j);
		PUSH_BACK_VECTOR(trans, NULL_TRANS_LST);
		for (size_t i = MIN_ASCII; i < MAX_ASCII; ++i) {
			bitset_t* next = dfa_edge(AT_VECTOR(states, j), i);
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
build_middle_table(vector_t* states) {
	vector_t* middle = NULL_VECT;
	for (size_t i = 1; i < SIZE_VECTOR(states); ++i) {
		bitset_t* set_state = (bitset_t*)AT_VECTOR(states, i);
		int j;
		while ((j = IT_NEXT(set_state)) != IT_NULL) {
			nfa_state_t* crt_state = STATE_AT(j);
			if (crt_state->beg_look) {
				if (!middle)
					{ middle = new_vector(); }
				PUSH_BACK_VECTOR(middle, (void*)i);
			}
		}
		IT_RESET(set_state);
	}
	return (middle);
}

static vector_t*
build_final_table(vector_t* states, vector_t* elst) {
	vector_t* final = new_vector();

	for (size_t i = 1; i < SIZE_VECTOR(states); ++i) {
		bitset_t* set_state = (bitset_t*)AT_VECTOR(states, i);
		int min_tok = 0;

		int it;
		while ((it = IT_NEXT(set_state)) != IT_NULL) {
			nfa_state_t* crt_state = STATE_AT(it);
			if (crt_state->final_type) {
				if (!min_tok || min_tok > crt_state->final_type)
					{ min_tok = crt_state->final_type; }
			}
		}
		if (min_tok) {
			char* name = ((spec_entry_t*)AT_VECTOR(elst, min_tok - 1))->name;

			PUSH_BACK_VECTOR(final, (void*)i);
			PUSH_BACK_VECTOR(final, name);
		}
	}
	return (final);
}

static void
precompute_epsilon_closure(nfa_state_t* state) {
    if (state->eclos)
        { return; }
    bitset_t* result_set = new_bitset();
    ADD_BITSET(result_set, GET_INDEX(state));
    epsilon_closure(result_set);
    state->eclos = result_set;
}

void
build_dfa_table(nfa_state_t* master, lexical_spec_t* spec) {
    static bool do_eclos = false;
    if (!do_eclos) {
        for (size_t i = 0; i < MAX_SIZE_STATE; ++i)
            { precompute_epsilon_closure(STATE_AT(i)); }
        do_eclos = true;
    }

	if (spec->trans) {
		foreach_vector(spec->trans, &del_trans_list);
		del_vector(spec->trans);
	}

	if (spec->middle)
		{ del_vector(spec->middle); }
	if (spec->final)
		{ del_vector(spec->final); }

	vector_t* states = NULL;

	spec->trans = build_state_table(master, &states);
	spec->middle = build_middle_table(states);
	spec->final = build_final_table(states, spec->entry_vect);

	foreach_vector(states, &del_bitset);
	del_vector(states);
}

#ifdef DFA_OPTIMIZE

static void
redirect_transition(vector_t* trans, long s1, long s2) {
	del_trans_list(AT_VECTOR(trans, s2));
	erase_vector(trans, s2);

	for (size_t i = 1; i < SIZE_VECTOR(trans); ++i) {
		trans_list_t* list = (trans_list_t*)AT_VECTOR(trans, i);
		redirect_trans_list(list, s1, s2);
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

void 
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
					(AT_VECTOR(finalt, fs1 + 1) == AT_VECTOR(finalt, fs2 + 1));
			
				bool nonfinal = (fs1 == -1 && fs2 == -1);
		
				if (final || nonfinal) {
					repeat = true;
					redirect_transition(trans, i, j);
					redirect_final(finalt, final, fs2, j);
				}
			}
		}
	} while (repeat);
}

#endif /* DFA_OPTIMIZE */
