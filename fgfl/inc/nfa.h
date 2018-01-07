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
#define STATE_AT(index)			(AT_VECTOR(record_nfa_state, (size_t)index))

#define MIN_ASCII				(0)
#define MAX_ASCII				(UCHAR_MAX + 1)

#define EPSILON					(MAX_ASCII + 1)

extern vector_t* record_nfa_state;

void del_nfa_record(void);
state_t* new_state(void);
int ast_to_nfa(spec_entry_t*);
int build_nfa(lexical_spec_t*);

#endif /* NFA_H */
