#ifndef NFA_H
#define NFA_H

#include <stddef.h>
#include <limits.h>

#include "fgfl.h"
#include "regex.h"

#define STATE_FINAL(state, tok)	(state->final = tok)
#define FREE_FRAG(f)			((FREE(f->tail)) , (FREE(f)))
#define CPY_EDGE(x, y)			((x->label = y->label),\
										(x->out_state = y->out_state))

#define PUSH_EDGE(st, ed)		((ed->next = st->trans) , (st->trans = ed))
#define INIT_EDGE(ed, l, o)		((ed->label = l) , (ed->out_state = o))
#define	MAX_STATE				(712)
#define STATE_AT(index)			(record_state[index])

#define MIN_ASCII				(0)
#define MAX_ASCII				(UCHAR_MAX + 1)

#define EPSILON					(MAX_ASCII + 1)

extern state_t* record_state[MAX_STATE];

void del_record(void);
state_t* state_at(size_t);

state_t* new_state(void);
nfa_frag_t* ast_to_nfa(regex_node_t*, int, bool);
int build_nfa(token_spec_t*);

#endif /* NFA_H */
