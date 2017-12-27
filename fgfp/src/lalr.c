#include "lalr.h"
#include "lr.h"

bitset_t*
mark_equiv_state(vector_t const* lr1_states) {
	if (EMPTY_VECTOR(lr1_states))
		{ return (NULL_BITSET); }

	bitset_t* seen_symbol = new_bitset();
	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		if (IS_PRESENT(seen_symbol, i))
			{ continue; }

		lr1_state_t* outer_state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
		for (size_t j = i + 1; j < SIZE_VECTOR(lr1_states); ++j) {
			lr1_state_t* inner_state = (lr1_state_t*)AT_VECTOR(lr1_states, j);

			if ((outer_state->hash_items == inner_state->hash_items)
					&& !cmp_lr0_state(outer_state->items, inner_state->items)) {

				ADD_BITSET(seen_symbol , j);
				if (!outer_state->equiv_state)
					{ outer_state->equiv_state = new_bitset(); }
				ADD_BITSET(outer_state->equiv_state, j);
			}
		}
	}
	return (seen_symbol);
}

static int
find_equiv_state(lr1_state_t* state, size_t i) {
	return (!IS_PRESENT(state->equiv_state, i));
}

vector_t*
gen_lalr1_states(cfg_t const* cfg) {
	vector_t* lr1_states = gen_lr1_states(cfg);
	bitset_t* seen_symbol = mark_equiv_state(lr1_states);

	IT_INVERT(seen_symbol);
	
	long i;
	while ((i = IT_NEXT(seen_symbol)) != IT_NULL) {
		int belong = get_index_vector(lr1_states, (void const*)i,
										&find_equiv_state);

		lr1_state_t* st1 = AT_VECTOR(lr1_states, (size_t)belong);
		lr1_state_t* st2 = AT_VECTOR(lr1_states, (size_t)i);

		merge_lr1_state(st1, st2);
		erase_vector(lr1_states, i);

		for (size_t j = 0; j < SIZE_VECTOR(lr1_states); ++j) {
			lr1_state_t* change_state = AT_VECTOR(lr1_states, j);
			redirect_trans_list(change_state->shift_lst, belong, (size_t)i);	
			redirect_trans_list(change_state->goto_lst, belong, (size_t)i);	
		}
	}

	del_bitset(seen_symbol);
	return (lr1_states);
}

