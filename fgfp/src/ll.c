#include "ll.h"
#include "cfg_production.h"
#include "utils.h"

bool
is_ll1(cfg_t const* cfg) {
	bool disjoint = true;
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = (symbol_t*)AT_VECTOR(cfg->non_terminal, i);
		if (nter->nullable) {
			disjoint = is_disjoint_bitset(nter->first, nter->follow);
			if (!disjoint)
				{ break; }
		}
		disjoint = disjoint_select_set(cfg, nter);
		if (!disjoint)
			{ break; }
	}
	return (disjoint);
}

vector_t*
gen_ll1_table(cfg_t const* cfg) {
	vector_t* ll_table = new_vector();

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = AT_VECTOR(cfg->non_terminal, i);
		PUSH_BACK_VECTOR(ll_table, NULL_TRANS_LST);

		int j;
		while ((j = IT_NEXT(nter->prod_lst)) != IT_NULL) {
			production_t* prod = AT_VECTOR(cfg->productions, j);
			int k;
			while ((k = IT_NEXT(prod->select_set)) != IT_NULL) {
				trans_list_t* lst = new_trans_list(((symbol_t*)
									AT_VECTOR(cfg->terminal, k))->index, j);
				lst->next = BACK_VECTOR(ll_table);
				BACK_VECTOR(ll_table) = lst;
			}

			IT_RESET(prod->select_set);

			if (!prod->can_get_rid)
				{ continue; }
			while ((k = IT_NEXT(prod->symbol_lhs->follow)) != IT_NULL) {
				trans_list_t* lst = new_trans_list(((symbol_t*)
								AT_VECTOR(cfg->terminal, k))->index, j);
				lst->next = BACK_VECTOR(ll_table);
				BACK_VECTOR(ll_table) = lst;
			}
			IT_RESET(prod->symbol_lhs->follow);
		}
		IT_RESET(nter->prod_lst);
	}
	return (ll_table);
}

