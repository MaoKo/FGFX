#include <stdio.h>
#include <string.h>

#include "cfg_production.h"
#include "cfg.h"
#include "error.h"
#include "utils.h"
#include "vector.h"

production_t*
new_production(symbol_t* lhs, cfg_t const* cfg) {
	production_t* prod = NEW(production_t, 1);

	if (!prod)
		{ return (NULL); }
	memset(prod, 0, sizeof(production_t));

	prod->symbol_lhs = lhs;
	prod->index = SIZE_VECTOR(cfg->productions);
	PUSH_BACK_VECTOR(cfg->productions, prod);

	return (prod);
}

void
del_production(production_t* prod) {
	if (prod) {
		list_rhs_t* it_list = prod->rhs_element;
		while (it_list) {
			list_rhs_t* next = it_list->next;
			FREE(it_list);
			it_list = next;
		}
		del_bitset(prod->select_set);
	}
	FREE(prod);
}

int
add_symbol_rhs(production_t* prod, symbol_t* symbol) {
	if (!prod)
		{ return (ERROR); }

	list_rhs_t* new_list = NEW(list_rhs_t, 1);
	if (!new_list)
		{ return (ERROR); }

	memset(new_list, 0, sizeof(list_rhs_t));
	new_list->pos = 1;
	new_list->symbol_rhs = symbol;

	if (!prod->rhs_element)
		{ prod->rhs_element = new_list; }
	else {
		list_rhs_t* crt_rhs = prod->rhs_element;
		while (crt_rhs->next) {
			crt_rhs = crt_rhs->next;
			++(new_list->pos);
		}
		crt_rhs->next = new_list;
	}
	return (DONE);
}

symbol_t*
last_symbol_in_prod(production_t const* prod) {
	if (!prod)
		{ return (NULL); }
	symbol_t* last_ter = NULL;
	list_rhs_t* list = prod->rhs_element;
	while (list) {	
		if (IS_TERMINAL(list->symbol_rhs))
			{ last_ter = list->symbol_rhs; }
		list = list->next;
	}
	return (last_ter);
}

int
preprocess_literal(cfg_t const* cfg) {
	if (!cfg)
		{ return (ERROR); }
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = (production_t*)AT_VECTOR(cfg->productions, i);
		if (prod->mimic_sym && (prod->mimic_sym->kind == LITERAL)) {
			prod->mimic_sym = (symbol_t*)AT_VECTOR(cfg->terminal,
											prod->mimic_sym->terminal_alias);
		}
		list_rhs_t* list = prod->rhs_element;
		while (list) {
			if (list->symbol_rhs->kind == LITERAL) {
				list->symbol_rhs = (symbol_t*)AT_VECTOR(cfg->terminal,
										list->symbol_rhs->terminal_alias);
			}
			list = list->next;
		}
	}
	return (DONE);
}

int
check_mimic_prod(cfg_t const* cfg) {
	if (!cfg)
		{ return (ERROR); }
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = (production_t*)AT_VECTOR(cfg->productions, i);
		if (prod->mimic_sym && !last_symbol_in_prod(prod)) {
			warnf(0, "The production %zu contain no terminal for the $MIMIC.",
							GET_INDEX(prod) + 1);
		}
	}
	return (DONE);
}

bool
list_is_nullable(list_rhs_t const* list) {
	if (!list)
		{ return (true); }
	while (list) {
		if (list->symbol_rhs->kind == TERMINAL)
			{ return (false); }
		else if (!list->symbol_rhs->nullable)
			{ return (false); }
		list = list->next;
	}
	return (true);
}

bool
production_is_nullable(production_t const* prod) {
	if (!prod)
		{ return (false); }
	return (list_is_nullable(prod->rhs_element));
}

bitset_t*
first_list_rhs_t(list_rhs_t const* list) {
	if (!list)
		{ return (NULL_BITSET); }
	bitset_t* select_set = new_bitset();
	while (list) {
		if (IS_TERMINAL(list->symbol_rhs)) {
			ADD_BITSET(select_set, GET_INDEX(list->symbol_rhs));
			break;
		}
		else {
			UNION_BITSET(select_set, list->symbol_rhs->first);
			if (!list->symbol_rhs->nullable)
				{ break; }
		}
		list = list->next;
	}
	return (select_set);
}

bitset_t*
first_production(production_t const* prod) {
	if (!prod)
		{ return (NULL_BITSET); }
	return (first_list_rhs_t(prod->rhs_element));
}

list_rhs_t const*
match_symbol_production(list_rhs_t const* list, symbol_t const* symbol) {
	if (!list)
		{ return (NULL); }
	while (list) {
		if (list->symbol_rhs == symbol)
			{ return (list); }
		list = list->next;
	}
	return (NULL);
}

vector_t*
stack_production_lhs(cfg_t const* cfg, symbol_t* nter) {
	if (!cfg)
		{ return (NULL); }
	vector_t* stack = new_vector();
	int i;
	while ((i = IT_NEXT(nter->prod_lst)) != IT_NULL)
		{ PUSH_BACK_VECTOR(stack, AT_VECTOR(cfg->productions, i)); }
	IT_RESET(nter->prod_lst);
	return (stack);
}

int
unreachable_production(cfg_t const* cfg) {
	if (!cfg)
		{ return (ERROR); }
	if (EMPTY_VECTOR(cfg->non_terminal))
		{ return (DONE); }

	bitset_t* nter_seen = new_bitset();
	vector_t* stack_prod = NULL;

	symbol_t* crt_symbol = AT_VECTOR(cfg->non_terminal, cfg->goal);
	ADD_BITSET(nter_seen, GET_INDEX(crt_symbol));

	stack_prod = stack_production_lhs(cfg, crt_symbol);
	while (!EMPTY_VECTOR(stack_prod)) {
		production_t* crt_prod = (production_t*)BACK_VECTOR(stack_prod);
		POP_BACK_VECTOR(stack_prod);

		list_rhs_t* list = crt_prod->rhs_element;
		while (list) {
			crt_symbol = list->symbol_rhs;
			if (IS_NON_TERMINAL(crt_symbol) && !IS_PRESENT(nter_seen,
												GET_INDEX(crt_symbol))) {
				ADD_BITSET(nter_seen, GET_INDEX(crt_symbol));
				move_vector(stack_prod, stack_production_lhs(cfg, crt_symbol));
			}
			list = list->next;
		}
	}
	
	int unreach = DONE;
	for (int i = SIZE_VECTOR(cfg->non_terminal) - 1; i >= 0; --i) {
		if (!IS_PRESENT(nter_seen, (size_t)i)) {
			unreach = ERROR;
			warnf(0, "The nonterminal %s is unreachable.",
				((symbol_t*)AT_VECTOR(cfg->non_terminal, i))->name);
			erase_vector(cfg->non_terminal, i);
		}
	} 

	del_bitset(nter_seen);
	del_vector(stack_prod);

	return (unreach);
}

static bool
nter_not_derive(cfg_t const* cfg, symbol_t* symbol, bitset_t* nter_seen) {
	if (IS_PRESENT(nter_seen, GET_INDEX(symbol)) || symbol->realizable)
		{ return (symbol->realizable);  }

	ADD_BITSET(nter_seen, GET_INDEX(symbol));

	vector_t* stack_prod = stack_production_lhs(cfg, symbol);
	for (size_t i = 0; i < SIZE_VECTOR(stack_prod); ++i) {
		production_t* crt_prod = (production_t*)AT_VECTOR(stack_prod, i);
		list_rhs_t* list = crt_prod->rhs_element;

		bool seq_real = true;
		while (list) {
			symbol_t* crt_symbol = list->symbol_rhs;
			if (IS_NON_TERMINAL(crt_symbol))
				{ seq_real &= nter_not_derive(cfg, crt_symbol, nter_seen); }
			list = list->next;
		}

		if (seq_real) {
			symbol->realizable = seq_real;
			break;
		}
	}
	del_vector(stack_prod);
	return (symbol->realizable);
}

static void
precompute_realizable(cfg_t const* cfg) {
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* crt_prod = (production_t*)AT_VECTOR(cfg->productions, i);
		list_rhs_t* list = crt_prod->rhs_element;
		bool whole_ter = true;
		while (list) {
			symbol_t* crt_symbol = list->symbol_rhs;
			if (!IS_TERMINAL(crt_symbol)) {
				whole_ter = false;
				break;
			}
			list = list->next;
		}
		LHS(crt_prod)->realizable = whole_ter;
	}
}

int
cfg_not_realizable(cfg_t const* cfg) {
	if (!cfg)
		{ return (ERROR); }

	precompute_realizable(cfg);

	int exit_status = DONE;
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* symbol = AT_VECTOR(cfg->non_terminal, i);
		if (symbol->realizable)
			{ continue; }

		if ((is_empty_bitset(symbol->first)) && (!symbol->nullable))
			{ symbol->realizable = false; }
		else {
			bitset_t* nter_seen = new_bitset();
			nter_not_derive(cfg, symbol, nter_seen);
			del_bitset(nter_seen);
		}

		if (!symbol->realizable) {
			errorf(0, "Non terminal %s derive no string at all.",
									symbol->name);
			exit_status = ERROR;
		}
	}
	return (exit_status);
}

bool
disjoint_select_set(cfg_t const* cfg, symbol_t* nter) {
	if (!cfg)
		{ return (true); }
	bool disjoint = true;
	vector_t* stack_prod = stack_production_lhs(cfg, nter);
	while (!EMPTY_VECTOR(stack_prod)) {
		production_t* prod = BACK_VECTOR(stack_prod);
		for (size_t j = 0; j < SIZE_VECTOR(stack_prod) - 1; ++j) {
			production_t* tmp_p = AT_VECTOR(stack_prod, j);
			if (!is_disjoint_bitset(prod->select_set, tmp_p->select_set)) {
				disjoint = false;
				break;
			}
		}
		if (!disjoint)
			{ break; }

		POP_BACK_VECTOR(stack_prod);	
	}

	del_vector(stack_prod);
	return (disjoint);
}

