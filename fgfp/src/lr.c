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
	if (item) {  }
//		{ del_bitset(item->lookahead); }
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
	if (crt_item->is_last
			|| !IS_NON_TERMINAL(crt_item->dot_pos->symbol_rhs))
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
	if (is_empty_bitset(goto_lr_state)) {
		del_bitset(goto_lr_state);
		return (NULL_BITSET);
	}
	return (closure(cfg, goto_lr_state));
}

static bitset_t*
move_item_next(cfg_t const* cfg, bitset_t* item_state, int index) {
	lr1_item_t* item = (lr1_item_t*)AT_VECTOR(record_item, index);
	if (item->is_last)
		{ return (NULL_BITSET); }
	int back = IT_BACK(item_state);
	IT_RESET(item_state);
	bitset_t* lrg = goto_lr(cfg, item_state, item->dot_pos->symbol_rhs);
	IT_SET(item_state, back);
	return (lrg);
}

static int
cmp_bitset(bitset_t* b1, bitset_t* b2) {
	return (!eq_bitset(b1, b2));
}

vector_t*
gen_lalr1_table(cfg_t const* cfg) {
	if (!cfg)
		{ return (NULL_VECT); }

	bitset_t* start_state = new_bitset();
	production_t* start_prod = BACK_VECTOR(cfg->productions);
	ADD_BITSET(start_state, (size_t)new_item(start_prod,
					start_prod->rhs_element));

	vector_t* lr1_states = new_vector();
	PUSH_BACK_VECTOR(lr1_states, closure(cfg, start_state));

	bool change;
	do {
		change = false;
		for (int i = SIZE_VECTOR(lr1_states) - 1; i >= 0; --i) {
			bitset_t* state = (bitset_t*)AT_VECTOR(lr1_states, i);
			int j;
			while ((j = IT_NEXT(state)) != -1) {
				bitset_t* next = move_item_next(cfg, state, j);
				if (next == NULL_BITSET)
					{ continue; }
				int index = get_index_vector(lr1_states,
					next, &cmp_bitset);
				if (index == -1) {
					PUSH_BACK_VECTOR(lr1_states, next);
					change = true;
				}
				else
					{ del_bitset(next); }
			}
			IT_RESET(state);
		}
	} while (change);
	return (lr1_states);
}

#ifdef PRINT_DEBUG
void print_item(bitset_t* item_set) {
	int i;
	while ((i = IT_NEXT(item_set)) != -1) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_item, i);
		printf("%s -> ", crt_item->prod->symbol_lhs->name);
		list_rhs* list = crt_item->prod->rhs_element;
		while (list) {
			if (list == crt_item->dot_pos)
				{ printf(". "); }
			printf("%s ", list->symbol_rhs->name);
			list = list->next;
		}
		if (crt_item->is_last)
			{ printf("."); }
		puts("");
	}
	IT_RESET(item_set);
}
#endif /* PRINT_DEBUG */

