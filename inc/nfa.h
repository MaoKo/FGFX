#ifndef NFA_H
#define NFA_H

#include "fgfl.h"
#include "vector.h"
#include "regex.h"

#define MARK_STATE_FINAL(state, tok)	(state_at(state)->final = tok)
#define ADD_EDGE2STATE(st, edg)		add_set(&state_at(st)->edges, edg)
#define MAKE_TRANS(sst, sym, fst)	ADD_EDGE2STATE(sst, edge(sym, fst))

#define NEW_STATE			-1

void new_record(void);
void del_record(void);
state_t* state_at(size_t);

int state(int);
edge_t* edge(int, int);

nfa_fragment_t* ast2nfa(node_ast_t*, int);
int merge_nfa_fragment(set_t*);
int NFAgen(token_spec_t*);

#endif /* NFA_H */
