#ifndef DFA_H
#define DFA_H

#include "fgfl.h"
#include "vector.h"

void edges(state_t*, int, bitset_t*);
bitset_t* epsilon_closure(bitset_t*);
bitset_t* dfa_edge(bitset_t*, int);
void equivalent_state(vector_t*, vector_t*);
void build_dfa_table(state_t*, vector_t**, vector_t**, vector_t*);

#endif /* DFA_H */
