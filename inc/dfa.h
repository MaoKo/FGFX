#ifndef GEN_DFA_H
#define GEN_DFA_H

#include "tokendef.h"
#include "out_gen.h"
#include "nfa.h"

set_t* edges(long, int);
set_t* closure(set_t**);
set_t* DFAedge(set_t*, int);
void DFAgen(token_spec_t*, char const*);

#endif /* GEN_DFA_H */
