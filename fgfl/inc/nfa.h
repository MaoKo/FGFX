#ifndef NFA_H
#define NFA_H

#include <stddef.h>

#include "fgfl.h"
#include "regex.h"

#define STATE_FINAL(state, tok)	(state->final = tok)
#define FREE_FRAG(f)			((FREE(f->tail)) , (FREE(f)))
#define CPY_EDGE(x, y)			((x->label = y->label),\
										(x->out_state = y->out_state))

#define PUSH_EDGE(st, ed)		((ed->next = st->trans) , (st->trans = ed))
#define INIT_EDGE(ed, l, o)		((ed->label = l) , (ed->out_state = o))
#define STATE_AT(index)			(AT_VECTOR(record_nfa_state, (size_t)index))
#define NULL_FRAG               (NULL)

extern vector_t* record_nfa_state;

void del_nfa_record(void);
state_t* new_state(void);
int build_nfa(lexical_spec_t*);

#endif /* NFA_H */
