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
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions) - 1; ++i) {
		production_t* prod = AT_VECTOR(cfg->productions, i);
		PUSH_BACK_VECTOR(ll_table, NULL_TRANS_LST);
		int j;
		while ((j = IT_NEXT(prod->select_set)) != -1) {
			trans_list_t* lst = new_trans_list(((symbol_t*)
				AT_VECTOR(cfg->terminal, j))->index, i);	
			lst->next = BACK_VECTOR(ll_table);
			SET_VECTOR(ll_table, i, lst);
		}
		IT_RESET(prod->select_set);
		if (prod->can_get_rid) {
			while ((j = IT_NEXT(prod->symbol_lhs->follow)) != -1) {
				trans_list_t* lst = new_trans_list(((symbol_t*)
					AT_VECTOR(cfg->terminal, j))->index, i);
				lst->next = BACK_VECTOR(ll_table);
				SET_VECTOR(ll_table, i, lst);
			}
			IT_RESET(prod->symbol_lhs->follow);
		}
	}
	return (ll_table);
}

