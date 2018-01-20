#ifndef DFA_H
#define DFA_H

#include "fgfl.h"

#include "vector.h"
#include "bitset.h"

#define DFA_STATE_AT(spec, i)   (AT_VECTOR(spec->states, (size_t)i))
#define NULL_DFA_STATE          (NULL)

#define START_GROUP             (1)

dfa_state_t* new_dfa_state(bitset_t*, vector_t*);
void del_dfa_state(dfa_state_t*);

void build_dfa_table(nfa_state_t*, lexical_spec_t*);

#endif /* DFA_H */
