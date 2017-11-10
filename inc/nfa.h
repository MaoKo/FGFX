#ifndef NFA_H
#define NFA_H

#include "fgfl.h"

#define	TNONE	0

nfa_fragment_t* reg_ast2nfa(node_ast_t*, int);
state_t* merge_nfa_fragment(set_t*);

#endif /* NFA_H */
