#ifndef GEN_DFA_H
#define GEN_DFA_H

#include "tokendef.h"
#include "out_gen.h"
#include "nfa.h"

void edges(state_t*, int, bitset_t*);
bitset_t* closure(bitset_t*);
bitset_t* DFAedge(bitset_t*, int);
void DFAgen(token_spec_t*, char const*);

#endif /* GEN_DFA_H */
