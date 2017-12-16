#include <string.h>
#include <stdio.h>

#include "lr.h"
#include "cfg.h"
#include "cfg_production.h"
#include "utils.h"
#include "vector.h"

static vector_t* record_item = NULL;

static void
del_item(lr1_item_t* item) {
	if (item)
		{ del_bitset(item->lookahead); }
	FREE(item);
}

void
del_record_item(void) {
	foreach_vector(record_item, &del_item);
	del_vector(record_item);
}

static int
cmp_lr1_item(lr1_item_t const* it1, lr1_item_t const* it2) {
	return (!((it1->prod == it2->prod)
			&& (it1->dot_pos == it2->dot_pos)));
}

int
new_item(production_t const* prod, list_rhs const* dot_pos) {
	if (!prod)
		{ return (ERROR); }
	if (!record_item)
		{ record_item = new_vector(); }
	lr1_item_t* item = NEW(lr1_item_t, 1);
	if (!item)
		{ return (ERROR); }
	memset(item, 0, sizeof(lr1_item_t));
	item->prod = prod;
	item->dot_pos = dot_pos;
	if (!item->dot_pos)
		{ item->is_last = true; }
	int index = get_index_vector(record_item, item, &cmp_lr1_item);
	if (index != -1) {
		del_item(item);
		return (index);
	}
	item->index = SIZE_VECTOR(record_item);
	PUSH_BACK_VECTOR(record_item, item);
	return (item->index);
}

static void
include_production_item(cfg_t const* cfg,
			bitset_t* dst, lr1_item_t* crt_item) {
	if (!IS_NON_TERMINAL(crt_item->dot_pos->symbol_rhs))
		{ return; }
	vector_t* target_prod = stack_production_lhs(cfg,
					crt_item->dot_pos->symbol_rhs);
	for (size_t i = 0; i < SIZE_VECTOR(target_prod); ++i) {
		production_t const* prod =
				(production_t*)AT_VECTOR(target_prod, i);
		int index_item = new_item(prod, prod->rhs_element);
		if (index_item != ERROR)
			{ ADD_BITSET(dst, (size_t)index_item); }
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
			lr1_item_t* crt_item = (lr1_item_t*)
					AT_VECTOR(record_item, i);
			include_production_item(cfg, item_set, crt_item);
		}
		IT_RESET(last);
	} while (!eq_bitset(item_set, last));
	del_bitset(last);
	return (item_set);
}

bitset_t*
goto_lr(cfg_t const* cfg, bitset_t* item_set, symbol_t const* match_symbol) {
	if (!item_set || !match_symbol)
		{ return (NULL); }
	(void)cfg;
	bitset_t* goto_lr_state = new_bitset();
	int i = 0;
	while ((i = IT_NEXT(item_set)) != -1) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_item, i);
		if (crt_item->dot_pos && (crt_item->dot_pos->symbol_rhs
				== match_symbol)) {
		int index_item = new_item(crt_item->prod,
						crt_item->dot_pos->next);
		if (index_item != ERROR)
			{ ADD_BITSET(goto_lr_state, (size_t)index_item); }
		}
	}
	IT_RESET(item_set);
	return (closure(cfg, goto_lr_state));
}

#ifdef PRINT_DEBUG
void print_item(bitset_t* item_set) {
	int i;
	while ((i = IT_NEXT(item_set)) != -1) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_item, i);
		printf("%s -> ", crt_item->prod->symbol_lhs->name);
		list_rhs* list = crt_item->prod->rhs_element;
		if (!list)
			{ printf("."); }
		while (list) {
			if (list == crt_item->dot_pos)
				{ printf(". "); }
			printf("%s ", list->symbol_rhs->name);
			list = list->next;
		}
		puts("");
	}
}
#endif /* PRINT_DEBUG */

