#include <string.h>
#include <stdio.h>

#include "lr.h"
#include "cfg.h"
#include "cfg_production.h"
#include "utils.h"
#include "vector.h"

static vector_t* record_lr0_item[HASH_LR0_ITEM] = {};
static vector_t* record_lr1_item = NULL;

static lr1_state_t*
new_lr1_state(bitset_t* item) {
	lr1_state_t* state = NEW(lr1_state_t, 1);
	if (!state)
		{ return (NULL); }
	memset(state, 0, sizeof(lr1_state_t));
	state->items = item;
	state->hash_items = hash_bitset(item);
	state->first_reach = true;
	return (state);
}

void
del_lr1_state(lr1_state_t* state) {
	if (state) {
		del_bitset(state->items);
		del_trans_list(state->shift_lst);
		del_trans_list(state->goto_lst);
		del_trans_list(state->reduces);
	}
	FREE(state);
}

static void
del_lr1_item(lr1_item_t* item) {
	if (item) {  }
		{ del_bitset(item->lookahead); }
	FREE(item);
}

void
del_record_item(void) {
	foreach_vector(record_lr1_item, &del_lr1_item);
	del_vector(record_lr1_item);

	for (size_t i = 0; i < HASH_LR0_ITEM; ++i) {
		if (!record_lr0_item[i])
			{ continue; }
		foreach_vector(record_lr0_item[i], &free);
		del_vector(record_lr0_item[i]);
	}
}

static inline int
hash_lr0_item(production_t const* prod, list_rhs const* dot_pos) {
	size_t hash = prod->index;
	if (dot_pos)
		{ hash += dot_pos->pos; }
	return (hash % HASH_LR0_ITEM);
}

static inline int
cmp_lr0_item(lr0_item_t const* it1, lr0_item_t const* it2) {
	return 	(!		((it1->prod == it2->prod)
				&&	(it1->dot_pos == it2->dot_pos)
			));
}

static inline int
cmp_lr1_item(lr1_item_t const* it1, lr1_item_t const* it2) {
	return 	(!  	((it1->base_item == it2->base_item)
				&&  (eq_bitset(it1->lookahead, it2->lookahead))
			));
}

static lr0_item_t*
new_lr0_item(production_t const* prod, list_rhs const* dot_pos) {
	if (!prod)
		{ return (NULL); }

	size_t hash = hash_lr0_item(prod, dot_pos);
	if (!record_lr0_item[hash])
		{ record_lr0_item[hash] = new_vector(); }

	for (size_t i = 0; i < SIZE_VECTOR(record_lr0_item[hash]); ++i) {
		lr0_item_t* target = (lr0_item_t*)AT_VECTOR(record_lr0_item[hash], i);
		if ((target->prod == prod) && (target->dot_pos == dot_pos))
			{ return (target); }
	}

	lr0_item_t* item = NEW(lr0_item_t, 1);
	if (!item)
		{ return (NULL); }

	memset(item, 0, sizeof(lr0_item_t));
	item->prod = prod;
	item->dot_pos = dot_pos;

/*
	int index = get_index_vector(record_lr0_item, item, &cmp_lr0_item);
	if (index != -1) {
		FREE(item);
		return ((lr0_item_t*)AT_VECTOR(record_lr0_item, index));
	}
*/

	if (!item->dot_pos)
		{ item->is_final = true; }

//	item->index = SIZE_VECTOR(record_lr0_item[hash]);
//	item->bucket = hash;

	PUSH_BACK_VECTOR(record_lr0_item[hash], item);
	return (item);
}

static lr1_item_t*
new_lr1_item_ptr(production_t const* prod,
					list_rhs const* dot_pos, bitset_t const* lookahead) {
	if (!prod)
		{ return (NULL); }
	if (!record_lr1_item)
		{ record_lr1_item = new_vector(); }
	lr1_item_t* item = NEW(lr1_item_t, 1);
	if (!item)
		{ return (NULL); }
	memset(item, 0, sizeof(lr1_item_t));
	if (!(item->base_item = new_lr0_item(prod, dot_pos)))
		{ return (NULL); }
	item->lookahead = dup_bitset(lookahead);
	return (item);
}

static int
new_lr1_item_record(lr1_item_t* item) {
	if (!item)
		{ return (ERROR); }

	int index = get_index_vector(record_lr1_item, item, &cmp_lr1_item);
	if (index != -1) {
		del_lr1_item(item);
		return (index);
	}

	item->index = SIZE_VECTOR(record_lr1_item);
	PUSH_BACK_VECTOR(record_lr1_item, item);

	return (item->index);
}

int
new_lr1_item(production_t const* prod,
					list_rhs const* dot_pos, bitset_t const* lookahead) {
	return (new_lr1_item_record(new_lr1_item_ptr(prod, dot_pos, lookahead)));
}

static int
check_exist_item(bitset_t* state_item, lr1_item_t* item, int kind) {
	int back = IT_BACK(state_item);
	IT_RESET(state_item);
	int find = -1;
	int i;
	while ((i = IT_NEXT(state_item)) != IT_NULL) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_lr1_item, i);
		if ((kind == LR1_ITEM && !cmp_lr1_item(crt_item, item))
				|| (kind == LR0_ITEM && !cmp_lr0_item(BASE_LR0(crt_item),
													 BASE_LR0(item)))) {
			find = i;
			break;
		}
	}
	IT_SET(state_item, back);
	return (find);
}

static void
include_new_item(cfg_t const* cfg,
			bitset_t* state_item, lr1_item_t* crt_item) {

	if (BASE_LR0(crt_item)->is_final
			|| !IS_NON_TERMINAL(DOT(BASE_LR0(crt_item))->symbol_rhs))
		{ return /* (DONE) */ ; }

	vector_t* target_prod = stack_production_lhs(cfg,
		DOT(BASE_LR0(crt_item))->symbol_rhs);

	bitset_t* target_first = first_list_rhs(DOT(BASE_LR0(crt_item))->next);
	if (target_first == NULL_BITSET)
		{ target_first = dup_bitset(crt_item->lookahead); }
	else if (list_is_nullable(DOT(BASE_LR0(crt_item))->next))
		{ UNION_BITSET(target_first, crt_item->lookahead); }

	//int include = 0;
	for (size_t i = 0; i < SIZE_VECTOR(target_prod); ++i) {
		production_t const* prod = (production_t*)AT_VECTOR(target_prod, i);
		lr1_item_t* add_item = new_lr1_item_ptr(prod,
							prod->rhs_element, target_first);
		if (!add_item)
			{ return /* (ERROR) */ ; }
		
		int lr0_exist = check_exist_item(state_item, add_item, LR0_ITEM);
		if (lr0_exist != -1) {
			lr1_item_t* new_it = (lr1_item_t*)
							AT_VECTOR(record_lr1_item, lr0_exist);
			if (cmp_lr1_item(add_item, new_it)) {
				MERGE_LR1_ITEM(add_item, new_it);
				OFF_BITSET(state_item, (size_t)lr0_exist);
			}
			else {
				del_lr1_item(add_item);
				continue;
			}
		}
		int index = new_lr1_item_record(add_item);
		if (index != ERROR) {
			ADD_BITSET(state_item, (size_t)index);
//			++include;
		}
	}

	del_bitset(target_first);
	del_vector(target_prod);
//	return (include);
}

bitset_t*
closure(cfg_t const* cfg, bitset_t* item_set) {
	if (!cfg || !item_set)
		{ return (NULL_BITSET); }
	bitset_t* last = NULL_BITSET;
//	bool change;
	do {
//		change = false;
		del_bitset(last);
		last = dup_bitset(item_set);
		int i;
		while ((i = IT_NEXT(last /* item_set */)) != IT_NULL) {
			lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_lr1_item, i);
			include_new_item(cfg, item_set, crt_item);
//			if (include_new_item(cfg, item_set, crt_item))
//				{ change = true; }
		}
		IT_RESET(last);
	} while (!eq_bitset(item_set, last));
	del_bitset(last);
//	IT_RESET(item_set);
	return (item_set);
}

bitset_t*
goto_lr(cfg_t const* cfg, bitset_t* item_set, symbol_t const* match_symbol) {
	if (!item_set || !match_symbol)
		{ return (NULL_BITSET); }
	bitset_t* goto_lr_state = new_bitset();
	int i = 0;
	while ((i = IT_NEXT(item_set)) != IT_NULL) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_lr1_item, i);
		if (DOT(BASE_LR0(crt_item))
				&& (DOT(BASE_LR0(crt_item))->symbol_rhs == match_symbol)) {

			int index_item = new_lr1_item(PROD(BASE_LR0(crt_item)),
					DOT(BASE_LR0(crt_item))->next, crt_item->lookahead);
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
move_dot_next(cfg_t const* cfg, lr1_state_t* state, int index) {
	lr1_item_t* item = (lr1_item_t*)AT_VECTOR(record_lr1_item, index);
	if (BASE_LR0(item)->is_final)
		{ return (NULL_BITSET); }
	else if (IS_TERMINAL(DOT(BASE_LR0(item))->symbol_rhs)
			&& IS_EOF(DOT(BASE_LR0(item))->symbol_rhs)) {
		state->accept = true;
		return (NULL_BITSET);
	}
	
	unsigned int input;
	symbol_t* target = DOT(BASE_LR0(item))->symbol_rhs;
	if (IS_TERMINAL(target)) {
		input = SHIFT(GET_INDEX(target));
		state->last_move = &(state->shift_lst);
	}
	else {
		input = GOTO(GET_INDEX(target));
		state->last_move  = &(state->goto_lst);
	}

	bool find = cmp_input_trans_list(*(state->last_move), input);
	if (!find) {
		trans_list_t* action = new_trans_list(input, 0);
		action->next = *(state->last_move);
		*(state->last_move) = action;
	}
	else
		{ return (NULL_BITSET); }

	int back = IT_BACK(state->items);
	IT_RESET(state->items);
	bitset_t* lrg = goto_lr(cfg, state->items,
								DOT(BASE_LR0(item))->symbol_rhs);
	IT_SET(state->items, back);
	return (lrg);
}

static int
cmp_lr_state(lr1_state_t* s1, bitset_t* b2, int kind) {
	if (count_elt_bitset(s1->items) != count_elt_bitset(b2))
		{ return (1); }
	int back = IT_BACK(s1->items);
	IT_RESET(s1->items);
	int eq = 0;
	int i;
	while ((i = IT_NEXT(s1->items)) != IT_NULL) {
		lr1_item_t* lr1 = (lr1_item_t*)AT_VECTOR(record_lr1_item, i);
		if (check_exist_item(b2, lr1, kind) == -1) {
			eq = 1;
			break;
		}
	}
	IT_SET(s1->items, back);
	return (eq);
}

int
cmp_lr0_state(lr1_state_t* s1, bitset_t* b2) {
	return (cmp_lr_state(s1, b2, LR0_ITEM));
}

int
cmp_lr1_state(lr1_state_t* s1, bitset_t* b2) {
	return (!eq_bitset(s1->items, b2));
}

vector_t*
gen_lr1_states(cfg_t const* cfg) {
	if (!cfg)
		{ return (NULL_VECT); }

	bitset_t* start_state = new_bitset();
	production_t* start_prod = BACK_VECTOR(cfg->productions);
	ADD_BITSET(start_state, (size_t)new_lr1_item(start_prod,
					start_prod->rhs_element, NULL_BITSET));

	vector_t* lr1_states = new_vector();
	PUSH_BACK_VECTOR(lr1_states, new_lr1_state(closure(cfg, start_state)));

	bool change;
	do {
		change = false;
//		if (SIZE_VECTOR(lr1_states) > 1000)
//			{ break; }
		for (int i = SIZE_VECTOR(lr1_states) - 1; i >= 0; --i) {
			lr1_state_t* state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
			if (!state->first_reach)
				{ break; }

			int j;
			while ((j = IT_NEXT(state->items)) != IT_NULL) {
				bitset_t* next = move_dot_next(cfg, state, j);
				if (next == NULL_BITSET)
					{ continue; }

				size_t hash_test = hash_bitset(next);
				size_t index;
				for (index = 0; index < SIZE_VECTOR(lr1_states);
						++index) {
					lr1_state_t* test = (lr1_state_t*)
									AT_VECTOR(lr1_states, index);
					if (test->hash_items == hash_test
							&& !cmp_lr1_state(test, next)) {
						break;
					}
				}

				//int index = get_index_vector(lr1_states, next, &cmp_lr1_state);
				if (index == SIZE_VECTOR(lr1_states)) {
					//index = SIZE_VECTOR(lr1_states);
					PUSH_BACK_VECTOR(lr1_states, new_lr1_state(next));
					change = true;
				}
				else
					{ del_bitset(next); }

				if (state->first_reach)
					{ (*(state->last_move))->state = index; }
			}
			IT_RESET(state->items);
			state->first_reach = false;
		}
	} while (change);

	return (lr1_states);
}

static int
check_conflict(lr1_state_t* state, trans_list_t* reduce_list, int kind) {
	trans_list_t* target_action = (kind == SHIFT_REDUCE)
									? state->shift_lst : state->reduces;
	while (target_action) {
		trans_list_t* reset_list = reduce_list;
		while (reset_list) {
			if ((kind == SHIFT_REDUCE) && (_SHIFT & target_action->input)
					&& ((_SHIFT ^ target_action->input) == reset_list->input))
				{ return (true); }
			else if ((kind == REDUCE_REDUCE)
					&& (target_action->input == reset_list->input))
				{ return (true); }
			reset_list = reset_list->next;
		}
		target_action = target_action->next;
	}
	return (false);
}

#if 0
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
		symbol_t* ter = (symbol_t*)AT_VECTOR(cfg->terminal, i);
		if (IS_LITERAL(ter))
			{ continue; }
		trans_list_t* new_reduce = new_trans_list(i,
						REDUCE(item->prod->index));
		new_reduce->next = state->reduces;
		state->reduces = new_reduce;
	}
}
#endif

static void
add_reduce_slr(lr1_state_t* state, lr1_item_t* item, size_t index) {
	symbol_t* symbol_lhs = PROD(BASE_LR0(item))->symbol_lhs;

	int i;
	trans_list_t* reduce_list = NULL;
	while ((i = IT_NEXT(symbol_lhs->follow)) != IT_NULL) {
		trans_list_t* next = new_trans_list(i,
									REDUCE(PROD(BASE_LR0(item))->index));
		next->next = reduce_list;
		reduce_list = next;
	}
	IT_RESET(symbol_lhs->follow);
	bool fail = false;
	if (check_conflict(state, reduce_list, SHIFT_REDUCE)) {
			fprintf(stderr, "Shift/Reduce (state %zu).\n", index);
			fail = true;
	}
	if (check_conflict(state, reduce_list, REDUCE_REDUCE)) {
		fprintf(stderr, "Reduce/Reduce (state %zu).\n", index);
		fail = true;
	}

	if (fail) {
		del_trans_list(reduce_list);
		return;
	}

	if (!state->reduces)
		{ state->reduces = reduce_list; }
	else
		{ append_trans_list(state->reduces, reduce_list); }
}


static void
add_reduce_lr1(lr1_state_t* state, lr1_item_t* item, size_t index) {
	int i;
	trans_list_t* reduce_list = NULL;
	while ((i = IT_NEXT(item->lookahead)) != IT_NULL) {
		trans_list_t* next = new_trans_list(i,
									REDUCE(PROD(BASE_LR0(item))->index));
		next->next = reduce_list;
		reduce_list = next;
	}
	IT_RESET(item->lookahead);		
	bool fail = false;
	if (check_conflict(state, reduce_list, SHIFT_REDUCE)) {
			fprintf(stderr, "Shift/Reduce (state %zu).\n", index);
			fail = true;
	}
	if (check_conflict(state, reduce_list, REDUCE_REDUCE)) {
		fprintf(stderr, "Reduce/Reduce (state %zu).\n", index);
		fail = true;
	}

	if (fail) {
		del_trans_list(reduce_list);
		return;
	}

	if (!state->reduces)
		{ state->reduces = reduce_list; }
	else
		{ append_trans_list(state->reduces, reduce_list); }
}

void 
compute_reduce_op(cfg_t const* cfg, vector_t* lr1_states) {
	(void)add_reduce_slr;
	if (!cfg || !lr1_states)
		{ return; }
	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		lr1_state_t* state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
		int j;
		while ((j = IT_NEXT(state->items)) != IT_NULL) {
			lr1_item_t* item = (lr1_item_t*)AT_VECTOR(record_lr1_item, j);
			if (BASE_LR0(item)->is_final)
				{ add_reduce_lr1(state, item, i); }
		}
		IT_RESET(state->items);
	}
}

#ifdef PRINT_DEBUG

void
print_lr0_item(lr0_item_t* lr0_item) {
	printf("%s -> ", PROD(lr0_item)->symbol_lhs->name);
	list_rhs* list = PROD(lr0_item)->rhs_element;
	while (list) {
		if (list == DOT(lr0_item))
			{ printf(". "); }
		printf("%s", list->symbol_rhs->name);
		if (list->next)
			{ printf(" "); }
		list = list->next;
	}
	if (lr0_item->is_final)
		{ printf(" ."); }
}

void
print_lr1_item(cfg_t const* cfg, lr1_item_t* lr1_item) {
	print_lr0_item(BASE_LR0(lr1_item));
	printf(", ");
	if (is_empty_bitset(lr1_item->lookahead))
		{ printf("???"); }
	else {
		printf("[");
		int j;
		bool first = true;
		while ((j = IT_NEXT(lr1_item->lookahead)) != IT_NULL) {
			symbol_t* ter = (symbol_t*)AT_VECTOR(cfg->terminal, j);
			if (!first)
				{ printf(", "); }
			printf("%s", ter->name);
			first = false;
		}
		IT_RESET(lr1_item->lookahead);
		printf("]");
	}	
}

void
print_state(cfg_t const* cfg, bitset_t* item_set) {
	int i;
	while ((i = IT_NEXT(item_set)) != IT_NULL) {
		lr1_item_t* crt_item = (lr1_item_t*)AT_VECTOR(record_lr1_item, i);
		print_lr1_item(cfg, crt_item);
		puts("");
	}
	IT_RESET(item_set);
}

void
print_debug_report(cfg_t const* cfg, vector_t const* lr1_states) {
	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		printf("===== State %zu =====\n", i);
		lr1_state_t* state = AT_VECTOR(lr1_states, i);
		if (state->accept)
			{ printf("(Accept): "); }
		print_state(cfg, state->items);
		trans_list_t* list = state->shift_lst;
		while (list) {
			if (_SHIFT & list->input) {
				printf("SHIFT(%s)", ((symbol_t*)
					AT_VECTOR(cfg->terminal, list->input ^ _SHIFT))->name);
			}
			else if (_GOTO & list->input) {
				printf("GOTO(%s)", ((symbol_t*)
					AT_VECTOR(cfg->non_terminal, list->input ^ _GOTO))->name);
			}
			printf(" into state %d.\n", list->state);
			list = list->next;
		}
		list = state->reduces;
		while (list) {
			printf("TOKEN(%s) => REDUCE(%d).\n",
				((symbol_t*)AT_VECTOR(cfg->terminal, list->input))->name,
				(_REDUCE ^ list->state) + 1);
			list = list->next;
		}
	}
}

#endif /* PRINT_DEBUG */

