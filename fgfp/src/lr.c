#include <string.h>
#include <stdio.h>

#include "lr.h"
#include "cfg.h"
#include "cfg_production.h"
#include "utils.h"
#include "vector.h"

static vector_t* record_item = NULL;

static lr1_state_t*
new_lr1_state(bitset_t* item) {
	lr1_state_t* state = NEW(lr1_state_t, 1);
	if (!state)
		{ return (NULL); }
	memset(state, 0, sizeof(lr1_state_t));
	state->items = item;
	state->first_reach = true;
	return (state);
}

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
		{ item->is_final = true; }
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
	if (crt_item->is_final
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
		{ return (NULL_BITSET); }
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
		{ return (NULL_BITSET); }
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
move_item_next(cfg_t const* cfg, lr1_state_t* state, int index) {
	lr1_item_t* item = (lr1_item_t*)AT_VECTOR(record_item, index);
	if (item->is_final)
		{ return (NULL_BITSET); }
	else if (IS_TERMINAL(item->dot_pos->symbol_rhs)
			&& IS_EOF(item->dot_pos->symbol_rhs)) {
		state->accept = true;
		return (NULL_BITSET);
	}
	
	if (state->first_reach) {
		trans_list_t* action;
		if (IS_TERMINAL(item->dot_pos->symbol_rhs)) {
			action = new_trans_list(SHIFT(
				GET_INDEX(item->dot_pos->symbol_rhs)), 0);
		}
		else {
			action = new_trans_list(GOTO(
				GET_INDEX(item->dot_pos->symbol_rhs)), 0);
		}
	
		action->next = state->edges;
		state->edges = action;
	}

	int back = IT_BACK(state->items);
	IT_RESET(state->items);
	bitset_t* lrg = goto_lr(cfg, state->items, item->dot_pos->symbol_rhs);
	IT_SET(state->items, back);
	return (lrg);
}

static int
cmp_lr1_state(lr1_state_t const* s1, bitset_t const* b2) {
	return (!eq_bitset(s1->items, b2));
}

vector_t*
gen_lr1_states(cfg_t const* cfg) {
	if (!cfg)
		{ return (NULL_VECT); }

	bitset_t* start_state = new_bitset();
	production_t* start_prod = BACK_VECTOR(cfg->productions);
	ADD_BITSET(start_state, (size_t)new_item(start_prod,
					start_prod->rhs_element));

	vector_t* lr1_states = new_vector();
	PUSH_BACK_VECTOR(lr1_states, new_lr1_state(closure(cfg, start_state)));

	bool change;
	do {
		change = false;
		for (int i = SIZE_VECTOR(lr1_states) - 1; i >= 0; --i) {
			lr1_state_t* state = (lr1_state_t*)
						AT_VECTOR(lr1_states, i);
			int j;
			while ((j = IT_NEXT(state->items)) != -1) {
				bitset_t* next = move_item_next(cfg, state, j);
				if (next == NULL_BITSET)
					{ continue; }
				int index = get_index_vector(lr1_states,
					next, &cmp_lr1_state);
				if (index == -1) {
					index = SIZE_VECTOR(lr1_states);
					PUSH_BACK_VECTOR(lr1_states,
							new_lr1_state(next));
					change = true;
				}
				else
					{ del_bitset(next); }
				if (state->first_reach)
					{ state->edges->state = index; }

			}
			IT_RESET(state->items);
			state->first_reach = false;
		}
	} while (change);
	return (lr1_states);
}

static void
add_reduce_lr0(cfg_t const* cfg, lr1_state_t* state,
					lr1_item_t* item, size_t index) {
	trans_list_t* edge = state->edges;
	while (edge) {
		if (_SHIFT & edge->input) {
			fprintf(stderr, "Shift/Reduce (state %zu).\n", index);
			return;
		}
		edge = edge->next;
	}
	if (state->reduces) {
		fprintf(stderr, "Reduce/Reduce (state %zu).\n", index);
		return;
	}
	for (size_t i = 0; i < SIZE_VECTOR(cfg->terminal); ++i) {
		trans_list_t* new_reduce = new_trans_list(i,
						REDUCE(item->prod->index));
		new_reduce->next = state->reduces;
		state->reduces = new_reduce;
	}
}

#if 0
static void
add_reduce_slr(cfg_t const* cfg, lr1_state_t* state, lr1_item_t* item) {
	
}

static void
add_reduce_lr1(cfg_t const* cfg, lr1_state_t* state, lr1_item_t* item) {
	
}
#endif

void 
compute_reduce_op(cfg_t const* cfg, vector_t* lr1_states) {
	if (!cfg || !lr1_states)
		{ return; }
	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		lr1_state_t* state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
		int j;
		while ((j = IT_NEXT(state->items)) != -1) {
			lr1_item_t* item = (lr1_item_t*)
					AT_VECTOR(record_item, j);
			if (item->is_final)
				{ add_reduce_lr0(cfg, state, item, i); }
		}
		IT_RESET(state->items);
	}
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
		if (crt_item->is_final)
			{ printf("."); }
		puts("");
	}
	IT_RESET(item_set);
}
#endif /* PRINT_DEBUG */

