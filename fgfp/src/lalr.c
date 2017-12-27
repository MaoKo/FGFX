#include "fgfp.h"
#include "bitset.h"
#include "lr.h"
#include "lalr.h"

void
mark_equiv_state(vector_t* lr1_states) {
	bitset_t* seen = new_bitset();
	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		if (IS_PRESENT(seen, i))
			{ continue; }

		lr1_state_t* outer_state = (lr1_state_t*)AT_VECTOR(lr1_states, i);

		for (size_t j = i + 1; j < SIZE_VECTOR(lr1_states); ++j) {
			lr1_state_t* inner_state = (lr1_state_t*)AT_VECTOR(lr1_states, j);

			if ((outer_state->hash_items == inner_state->hash_items)
					&& !cmp_lr0_state(outer_state->items, inner_state->items)) {
				ADD_BITSET(seen , j);

				if (!outer_state->equiv_state)
					{ outer_state->equiv_state = new_bitset(); }
				ADD_BITSET(outer_state->equiv_state, j);
			}
		}
	}
	del_bitset(seen);
}
