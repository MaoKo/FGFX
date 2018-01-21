#include <string.h>

#include "dfa_minimization.h"
#include "regex.h"
#include "nfa.h"
#include "dfa.h"

static vector_t*
init_groups(lexical_spec_t* spec) {
    vector_t* groups = new_vector();
    bitset_t* non_final_group = new_bitset();

    //First select non final state 
    for (size_t i = 1; i < SIZE_VECTOR(spec->states); ++i) {
        dfa_state_t* crt_state = DFA_STATE_AT(spec, i);
        if (!crt_state->is_final)
            { ADD_BITSET(non_final_group, GET_INDEX(crt_state)); }
    }
    if (!is_empty_bitset(non_final_group))
        { PUSH_BACK_VECTOR(groups, non_final_group); }
    bitset_t* seen_final = new_bitset();
   
    //Second select final state
    for (size_t i = 1; i < SIZE_VECTOR(spec->states); ++i) {
        dfa_state_t* crt_state = DFA_STATE_AT(spec, i);
        if (!crt_state->is_final)
                { continue; }
        else if (IS_PRESENT(seen_final, GET_INDEX(crt_state)))
            { continue; }
        bitset_t* final_group = new_bitset();
 
        for (size_t j = i; j < SIZE_VECTOR(spec->states); ++j) {
            dfa_state_t* cmp_state = DFA_STATE_AT(spec, j);

            if ((cmp_state->final_anchor_entry == crt_state->final_anchor_entry)
                    && (cmp_state->final_entry == crt_state->final_entry)) {

                ADD_BITSET(final_group, GET_INDEX(cmp_state));
                ADD_BITSET(seen_final, GET_INDEX(cmp_state));

                cmp_state->group = SIZE_VECTOR(groups) + 1;
            }
        }
        PUSH_BACK_VECTOR(groups, final_group);
    }
    return (groups);
}

static void
merge_dfa_state(lexical_spec_t* spec, vector_t* groups) {
    vector_t* new_states = new_vector();
    PUSH_BACK_VECTOR(new_states, NULL_DFA_STATE);

    for (size_t i = 0; i < SIZE_VECTOR(groups); ++i) {
        bitset_t* crt_group = (bitset_t*)AT_VECTOR(groups, i);
        dfa_state_t* crt_state = DFA_STATE_AT(spec, (size_t)IT_NEXT(crt_group));

        trans_list_t* list = crt_state->trans;
        while (list) {
            dfa_state_t* out_state = DFA_STATE_AT(spec, list->state);
            if (out_state)
                { list->state = out_state->group; }
            list = list->next;
        }

        dfa_state_t* new_state = new_dfa_state(NULL_BITSET, new_states);
        memcpy(new_state, crt_state, sizeof(*crt_state));

        new_state->set_state = NULL_BITSET;
        crt_state->trans = NULL_TRANS_LST;
    }

    foreach_vector(spec->states, &del_dfa_state);
    del_vector(spec->states);

    foreach_vector(groups, &del_bitset);
    del_vector(groups);

    spec->states = new_states;
}

static size_t
input_to_state(trans_list_t const* list, size_t input) {
    while (list) {
        if ((size_t)list->input == input)
            { return (list->state); }
        list = list->next;
    }
    return (0);
}

void
minimizing_dfa(lexical_spec_t* spec) {
    if (!spec)
        { return; }
    vector_t* groups = init_groups(spec);

    int next_group_id = SIZE_VECTOR(groups);
    bool change;

    do {
        change = false;

        bitset_t* new_group = NULL_BITSET;
        ++next_group_id;

        for (int i = SIZE_VECTOR(groups) - 1; i >= 0; --i) {
            bitset_t* crt_group = (bitset_t*)AT_VECTOR(groups, (size_t)i);
            if (count_elt_bitset(crt_group) == 1)
                { continue; }

            int first = IT_NEXT(crt_group);
            int next;

            while ((next = IT_NEXT(crt_group)) != IT_NULL) {
                dfa_state_t* first_st = DFA_STATE_AT(spec, first);
                dfa_state_t* next_st = DFA_STATE_AT(spec, next);

                for (size_t j = MIN_ASCII; j < MAX_ASCII; ++j) {
                    int first_i = input_to_state(first_st->trans, j);
                    int next_i = input_to_state(next_st->trans, j);

                    if (first_i != next_i) {
                        dfa_state_t* first_out = DFA_STATE_AT(spec, first_i);
                        dfa_state_t* next_out = DFA_STATE_AT(spec, next_i);

                        if ((!first_out || !next_out)
                                || (first_out->group != next_out->group)) {
                            next_st->group = next_group_id;

                            if (!new_group) {
                                new_group = new_bitset();
                                change = true;
                            }

                            ADD_BITSET(new_group, (size_t)next);
                            OFF_BITSET(crt_group, (size_t)next);

                            break;
                        }
                    }
                }
            }
            IT_RESET(crt_group);
        }
        if (new_group)
            { PUSH_BACK_VECTOR(groups, new_group); }
    } while (change);
    merge_dfa_state(spec, groups);
}

