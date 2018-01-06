#ifndef DFA_H
#define DFA_H

#include "fgfl.h"
#include "vector.h"

void equivalent_state(vector_t*, vector_t*);
void build_dfa_table(token_spec_t*, vector_t**, vector_t**);

#endif /* DFA_H */
