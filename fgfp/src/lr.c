#include <string.h>
#include <stdio.h>

#include "lr.h"
#include "cfg.h"
#include "cfg_production.h"
#include "utils.h"
#include "vector.h"

static vector_t* record_item = NULL;

int
new_item(production_t const* prod) {
	if (!prod)
		{ return (ERROR); }
	if (!record_item)
		{ record_item = new_vector(); }
	for (size_t i = 0; i < SIZE_VECTOR(record_item); ++i) {
		lr1_item* item = (lr1_item*)AT_VECTOR(record_item, i);
		if (item->dot_pos == prod->rhs_element)
			{ return (i); }	
	}
	lr1_item* item = NEW(lr1_item, 1);
	if (!item)
		{ return (ERROR); }
	memset(item, 0, sizeof(lr1_item));
	item->dot_pos = prod->rhs_element;
	if (!item->dot_pos)
		{ item->is_last = true; }
	item->index = SIZE_VECTOR(record_item);
	PUSH_BACK_VECTOR(record_item, item);
	return (item->index);
}

static void
del_item(lr1_item* item) {
	if (item)
		{ del_bitset(item->lookahead); }
	FREE(item);
}

void
del_record_item(void) {
	foreach_vector(record_item, &del_item);
	del_vector(record_item);
}

static void
include_production_item(cfg_t const* cfg, bitset_t* dst, lr1_item* crt_item) {
	if (!IS_NON_TERMINAL(crt_item->dot_pos->symbol_rhs))
		{ return; }
	vector_t* target_prod = stack_production_lhs(cfg,
					crt_item->dot_pos->symbol_rhs);
	for (size_t i = 0; i < SIZE_VECTOR(target_prod); ++i) {
		int index_item = new_item(AT_VECTOR(target_prod, i));
		ADD_BITSET(dst, (size_t)index_item);	
	}
}

bitset_t*
closure(cfg_t const* cfg, bitset_t* item_set) {
	if (!cfg || !item_set || !record_item)
		{ return (NULL); }
	bitset_t* last = NULL_BITSET;
	do {
		del_bitset(last);
		last = dup_bitset(item_set);
		int i;
		while ((i = IT_NEXT(last)) != -1) {
			lr1_item* crt_item = (lr1_item*)
					AT_VECTOR(record_item, i);
			include_production_item(cfg, item_set, crt_item);
		}
		IT_RESET(last);
	} while (!eq_bitset(item_set, last));
	del_bitset(last);
	return (item_set);
}

