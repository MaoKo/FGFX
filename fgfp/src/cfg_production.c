#include <stdio.h>
#include <string.h>

#include "cfg_production.h"
#include "cfg.h"
#include "utils.h"
#include "vector.h"

production_t*
new_production(symbol_t* lhs) {
	production_t* prod = NEW(production_t, 1);
	if (!prod)
		{ return (NULL); }
	memset(prod, 0, sizeof(production_t));
	prod->symbol_lhs = lhs;
	return (prod);
}

void
del_production(production_t* prod) {
	if (prod) {
		list_rhs* it_list = prod->rhs_element;
		while (it_list) {
			list_rhs* next = it_list->next;
			FREE(it_list);
			it_list = next;
		}
		del_bitset(prod->select_set);
	}
	FREE(prod);
}

int
add_symbol_rhs(production_t* prod, symbol_t* symbol) {
	list_rhs* new_list = NEW(list_rhs, 1);
	if (!new_list)
		{ return (ERROR); }
	new_list->symbol_rhs = symbol;
	new_list->next = NULL;
	if (!prod->rhs_element)
		{ prod->rhs_element = new_list; }
	else {
		list_rhs* crt_rhs = prod->rhs_element;
		while (crt_rhs->next)
			{ crt_rhs = crt_rhs->next; }
		crt_rhs->next = new_list;
	}
	return (DONE);
}

bool
production_is_nullable(production_t const* prod) {
	if (!prod)
		{ return (false); }
	if (!prod->rhs_element)
		{ return (true); }
	list_rhs const* list = prod->rhs_element;
	while (list) {
		if (list->symbol_rhs->kind == TERMINAL)
			{ return (false); }
		else if (!list->symbol_rhs->nullable)
			{ return (false); }
		list = list->next;
	}
	return (true);
}

bitset_t*
first_production(production_t const* prod) {
	if (!prod || !(prod->rhs_element))
		{ return (NULL_BITSET); }
	bitset_t* select_set = new_bitset();
	list_rhs* list = prod->rhs_element;
	while (list) {
		if (IS_TERMINAL(list->symbol_rhs)) {
			ADD_BITSET(select_set, list->symbol_rhs->index);
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

list_rhs const*
match_symbol_production(list_rhs const* list, symbol_t const* symbol) {
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
	while ((i = IT_NEXT(nter->prod_lst)) != -1)
		{ PUSH_BACK_VECTOR(stack, AT_VECTOR(cfg->productions, i)); }
	IT_RESET(nter->prod_lst);
	return (stack);
}

int
unreachable_production(cfg_t const* cfg) {
	if (!cfg)
		{ return (ERROR); }
	bitset_t* nter_seen = new_bitset();
	vector_t* stack_prod = NULL;

	symbol_t* crt_symbol = AT_VECTOR(cfg->non_terminal, cfg->goal);
	ADD_BITSET(nter_seen, crt_symbol->index);

	stack_prod = stack_production_lhs(cfg, crt_symbol);

	while (!EMPTY_VECTOR(stack_prod)) {
		production_t* crt_prod = BACK_VECTOR(stack_prod);
		POP_BACK_VECTOR(stack_prod);
		list_rhs* list = crt_prod->rhs_element;
		while (list) {
			crt_symbol = list->symbol_rhs;
			if (IS_NON_TERMINAL(crt_symbol) && !IS_PRESENT(nter_seen,
						crt_symbol->index)) {
				ADD_BITSET(nter_seen, crt_symbol->index);
				move_vector(stack_prod,
					stack_production_lhs(cfg, crt_symbol));
			}
			list = list->next;
		}
	}
	
	int unreach = DONE;
	for (int i = SIZE_VECTOR(cfg->non_terminal) - 1; i >= 0; --i) {
		if (!IS_PRESENT(nter_seen, (size_t)i)) {
			unreach = ERROR;
			fprintf(stderr, "Warning %s unreachable.\n",
				((symbol_t*)AT_VECTOR(cfg->non_terminal, i))->name);
			erase_vector(cfg->non_terminal, i);
		}
	} 

	del_bitset(nter_seen);
	del_vector(stack_prod);

	return (unreach);
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
			if (!is_disjoint_bitset(prod->select_set,
						tmp_p->select_set)) {
				disjoint = false;
				goto exit;
			}
		}
		POP_BACK_VECTOR(stack_prod);	
	}
exit:
	del_vector(stack_prod);
	return (disjoint);
}

