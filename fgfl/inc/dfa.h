#ifndef DFA_H
#define DFA_H

#include "fgfl.h"
#include "vector.h"

void equivalent_state(vector_t*, vector_t*);
void build_dfa_table(nfa_state_t*, lexical_spec_t*);

#endif /* DFA_H */
