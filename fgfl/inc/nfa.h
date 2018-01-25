#ifndef NFA_H
#define NFA_H

#include <stddef.h>

#include "fgfl.h"

#define STATE_FINAL(state, tok) ((state)->final_type = (tok))
#define STATE_ANCHOR(state)     ((state)->is_anchor = true)

#define NFA_STATE_AT(index)     (AT_VECTOR(record_nfa_state, (size_t)index))
#define AUTOMATON_AT(index)     (AT_VECTOR(record_nfa_automata, (size_t)index))

#define MAX_SIZE_STATE          (SIZE_VECTOR(record_nfa_state))

#define NULL_AUTOMATON          (NULL)
#define NULL_NFA_STATE          (NULL)

#define EDGE_AUTOMATA           (-1)
#define EDGE_CLASS              (-2)
#define EDGE_BOTH               (-3)
#define NO_EDGE                 (-4)

#define NO_FINAL                (-1)

extern vector_t* record_nfa_state;
extern vector_t* record_nfa_automata;

void del_nfa_record(void);
int build_nfa(lexical_spec_t*);

#endif /* NFA_H */
