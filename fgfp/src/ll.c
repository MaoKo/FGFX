#include "ll.h"
#include "cfg_production.h"
#include "utils.h"
#include <stdio.h>

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
	for (size_t i = 1; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = AT_VECTOR(cfg->productions, i);
		int j;
		while ((j = IT_NEXT(prod->select_set)) != -1) {
			printf("[%s][T%s]=%zu\n", prod->symbol_lhs->name,
				((symbol_t*)AT_VECTOR(cfg->terminal, j))->name, i);
		}
		if (prod->can_get_rid) {
			while ((j = IT_NEXT(prod->symbol_lhs->follow)) != -1) {
				printf("[%s][T%s]=%zu\n", prod->symbol_lhs->name,
					((symbol_t*)
				AT_VECTOR(cfg->terminal, j))->name, i);
			}
			IT_RESET(prod->symbol_lhs->follow);
		}
	}
	return (NULL);
}

