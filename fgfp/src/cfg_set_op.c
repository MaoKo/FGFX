#include "cfg_set_op.h"
#include "cfg_production.h"
#include "cfg.h"
#include "bitset.h"

void
detect_nullable(cfg_t* cfg) {
	if (!cfg)
		{ return; }
	bool change;
	do {
		change = false;
		for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
			production_t* prod = (production_t*)
						AT_VECTOR(cfg->productions, i);
			if (!(prod->can_get_rid) && production_is_nullable(prod)) {
				change = prod->symbol_lhs->nullable = true;
				prod->can_get_rid = true;
			}
		}
	} while (change);
}

void
compute_first(cfg_t* cfg) {
	if (!cfg)
		{ return; }
	bool change;
	do {
		change = false;
		for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
			production_t* prod = (production_t*)
						AT_VECTOR(cfg->productions, i);
			bitset_t* first = first_production(prod);
			if (!is_subset_bitset(prod->select_set, first)) {
				if (!prod->symbol_lhs->first)
					{ prod->symbol_lhs->first = new_bitset(); }
				if (!prod->select_set)
					{ prod->select_set = new_bitset(); }
				UNION_BITSET(prod->symbol_lhs->first, first);
				UNION_BITSET(prod->select_set, first);
				change = true;
			}
			del_bitset(first);
		}
	} while (change);
}

static int
follow_terminal(symbol_t* non_ter, size_t ter_index) {
	if (!IS_PRESENT(non_ter->follow, ter_index)) {
		if (!non_ter->follow)
			{ non_ter->follow = new_bitset(); }
		ADD_BITSET(non_ter->follow, ter_index);
		return (true);
	}
	return (false);
}

static int
follow_non_terminal(symbol_t* non_ter, symbol_t const* nter_right) {
	if (!is_subset_bitset(non_ter->follow, nter_right->first)) {
		if (!non_ter->follow)
			{ non_ter->follow = new_bitset(); }
		UNION_BITSET(non_ter->follow, nter_right->first);
		return (true);
	}
	return (false);
}

static int
follow_symbol_rhs(production_t const* prod, symbol_t* non_ter,
						list_rhs_t const* list) {
	while (list) {
		if (IS_TERMINAL(list->symbol_rhs)) {
			return (follow_terminal(non_ter,
					list->symbol_rhs->index));
		}
		else {
			bool change = follow_non_terminal(non_ter,
							list->symbol_rhs);
			if (!list->symbol_rhs->nullable)
				{ return (change); }
		}
		list = list->next;
	}	
	if (!is_subset_bitset(non_ter->follow, prod->symbol_lhs->follow)) {
		if (!non_ter->follow)
			{ non_ter->follow = new_bitset(); }
		UNION_BITSET(non_ter->follow, prod->symbol_lhs->follow);
		return (true);
	}
	return (false);
}

static int
follow_symbol(symbol_t* non_ter, cfg_t* cfg) {
	bool change = false;
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = AT_VECTOR(cfg->productions, i);
		list_rhs_t const* list = prod->rhs_element;
		while ((list = match_symbol_production(list, non_ter))) {
			list = list->next;
			if (follow_symbol_rhs(prod, non_ter, list))
				{ change = true; }
		}
	}
	return (change);
}

void
compute_follow(cfg_t* cfg) {
	if (!cfg)
		{ return; }
	bool change;
	do {
		change = false;
		for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
			if (follow_symbol(AT_VECTOR(cfg->non_terminal, i), cfg))
				{ change = true; }
		}
	} while (change);
}


