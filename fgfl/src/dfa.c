#include <string.h>

#include "dfa.h"
#include "gen_dfa.h"
#include "lexical_spec.h"
#include "nfa.h"
#include "utils.h"

static void
nfa_automaton_edges(nfa_automaton_t* nfa_m, int symbol, bitset_t* result) {
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
            nfa_automaton_edges(nfa_m->left, symbol, result);
            break;

        case NFA_MASTER: ;
                int i;
                while ((i = IT_NEXT(nfa_m->out_automata)) != IT_NULL) {
                    nfa_automaton_t* crt_nfa_m = AUTOMATON_AT(i);
                    nfa_automaton_edges(crt_nfa_m, symbol, result);
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
            nfa_automaton_edges(state->edge, symbol, result);
            if (state->edge2)
                { nfa_automaton_edges(state->edge2, symbol, result); }
            break;

        case EDGE_CLASS:
            if ((symbol != EPSILON)
                                && (IS_PRESENT(state->class, (size_t)symbol)))
                { ADD_BITSET(result, GET_INDEX(state->out_state)); }
            break;

        case EDGE_BOTH:
            nfa_automaton_edges(state->edge, symbol, result);
            if (symbol == EPSILON)
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

static dfa_state_t*
new_dfa_state(bitset_t* set_state) {
    dfa_state_t* state = NEW(dfa_state_t, 1);

    if (!state)
        { return (NULL_DFA_STATE); }
    memset(state, 0, sizeof(*state));

    state->set_state = set_state;
    state->hash_state = hash_bitset(set_state);

    state->group = START_GROUP;
    return (state);
}

void
del_dfa_state(dfa_state_t* dfa_state) {
    if (dfa_state) {
        del_bitset(dfa_state->set_state);
        del_trans_list(dfa_state->trans);
    }
    FREE(dfa_state);
}

static vector_t*
build_state_table(nfa_state_t* master) {
    vector_t* states = new_vector();
    
    PUSH_BACK_VECTOR(states, NULL_DFA_STATE);
    PUSH_BACK_VECTOR(states, new_dfa_state(dup_bitset(master->eclos)));
    
    size_t j = 1;
    size_t p = 1;

    while (j <= p) {
        dfa_state_t* crt_state = (dfa_state_t*)AT_VECTOR(states, j);
        for (size_t i = MIN_ASCII; i < MAX_ASCII; ++i) {
            bitset_t* next = dfa_edge(crt_state->set_state, i);
            if (next == NULL_BITSET)
                { continue; }

            size_t hash_new_set = hash_bitset(next);

            trans_list_t* new_trans_list = NEW(trans_list_t, 1);
            new_trans_list->input = i;

            size_t l = 1;
            for (; l <= p; ++l) {
                dfa_state_t* cmp_state = (dfa_state_t*)AT_VECTOR(states, l);
                if ((hash_new_set == cmp_state->hash_state)
                        && (eq_bitset(next, cmp_state->set_state)))
                    { break; }
            }
            if (l <= p) {
                new_trans_list->state = l;
                del_bitset(next);
            }
            else {
                new_trans_list->state = ++p;
                PUSH_BACK_VECTOR(states, new_dfa_state(next));
            }
            new_trans_list->next = crt_state->trans;
            crt_state->trans = new_trans_list;
        }
        ++j;
    }

    return (states);
}

static void
build_middle_table(vector_t* states) {
    for (size_t i = 1; i < SIZE_VECTOR(states); ++i) {
        dfa_state_t* crt_state = (dfa_state_t*)AT_VECTOR(states, i);
        int j;
        while ((j = IT_NEXT(crt_state->set_state)) != IT_NULL) {
            nfa_state_t* nfa_state = STATE_AT(j);
            if (nfa_state->beg_look)
                { crt_state->middle = true; }
        }
        IT_RESET(crt_state->set_state);
    }
}

static size_t
build_final_table(vector_t* states) {
    size_t count_final = 0;
    for (size_t i = 1; i < SIZE_VECTOR(states); ++i) {
        dfa_state_t* crt_state = (dfa_state_t*)AT_VECTOR(states, i);
        int min_tok = NO_FINAL;

        int it;
        while ((it = IT_NEXT(crt_state->set_state)) != IT_NULL) {
            nfa_state_t* nfa_state = STATE_AT(it);
            if (nfa_state->final_type != NO_FINAL) {
                if ((min_tok == NO_FINAL)
                                    || (min_tok > nfa_state->final_type))
                    { min_tok = nfa_state->final_type; }
            }
        }
        IT_RESET(crt_state->set_state);

        if (min_tok != NO_FINAL) {
            crt_state->final_entry = min_tok;
            crt_state->group = FINAL_GROUP;
            ++count_final;
        }
    }
    return (count_final);
}

static inline void
precompute_epsilon_closure(nfa_state_t* state) {
    if (state->eclos)
        { return; }
    bitset_t* result_set = new_bitset();
    ADD_BITSET(result_set, GET_INDEX(state));
    state->eclos = epsilon_closure(result_set);
}

void
build_dfa_table(nfa_state_t* master, lexical_spec_t* spec) {
    static bool do_eclos = false;
    if (!do_eclos) {
        for (size_t i = 0; i < MAX_SIZE_STATE; ++i)
            { precompute_epsilon_closure(STATE_AT(i)); }
        do_eclos = true;
    }

    if (spec->states) {
        foreach_vector(spec->states, &del_dfa_state);
        del_vector(spec->states);
    }

    spec->states = build_state_table(master);
    build_middle_table(spec->states);
    spec->size_final = build_final_table(spec->states);
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
