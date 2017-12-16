#ifndef LR_H
#define LR_H

#include "fgfp.h"
#include "bitset.h"
#include "vector.h"

enum {
	_SHIFT	= 0x1000,
	_GOTO	= 0x2000,
};

#define SHIFT(x)	(x | _SHIFT)
#define GOTO(x)		(x | _GOTO)

void del_record_item(void);
int new_item(production_t const*, list_rhs const*);
bitset_t* closure(cfg_t const*, bitset_t*);
bitset_t* goto_lr(cfg_t const*, bitset_t*, symbol_t const*);
vector_t* gen_lalr1_table(cfg_t const*);
#ifdef PRINT_DEBUG
void print_item(bitset_t*);
#endif /* PRINT_DEBUG */

#endif /* LR_H */
