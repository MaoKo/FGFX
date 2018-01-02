#ifndef LR_H
#define LR_H

#include "fgfp.h"
#include "bitset.h"
#include "vector.h"

enum {
	_SHIFT	= 0x1000,
	_GOTO	= 0x2000,
	_REDUCE	= 0x4000,
};

#define SHIFT(x)		(x | _SHIFT)
#define GOTO(x)			(x | _GOTO)
#define REDUCE(x)		(x | _REDUCE)

#define SHIFT_REDUCE	1
#define REDUCE_REDUCE	2

// CMP Check
#define LR0_ITEM		1
#define LR1_ITEM		2

#define DOT(item)		((item)->dot_pos)
#define PROD(item)		((item)->prod)
#define	CORE(item)		((item)->core)

#define HASH_LR			50
#define MERGE_LR1_ITEM(lr1, lr2)	(UNION_BITSET((lr1)->lookahead,\
										(lr2)->lookahead))

void del_lr1_state(lr1_state_t*);
void del_record(void);
int new_item(production_t const*, list_rhs_t const*, bitset_t const*);
void merge_lr1_state(lr1_state_t*, lr1_state_t*);
bitset_t* closure(cfg_t const*, bitset_t*);
bitset_t* goto_lr(cfg_t const*, bitset_t*, symbol_t const*);
int cmp_lr0_state(bitset_t*, bitset_t*);
int cmp_lr1_state(bitset_t*, bitset_t*);
vector_t* build_lr1_states(cfg_t const*);
void compute_reduce_op(cfg_t const*, vector_t*);
#ifdef PRINT_DEBUG
void print_lr0_item(lr0_item_t*);
void print_lr1_item(cfg_t const*, lr1_item_t*);
void print_state(cfg_t const*, bitset_t*);
void print_debug_report(cfg_t const*, vector_t const*);
#endif /* PRINT_DEBUG */

#endif /* LR_H */
