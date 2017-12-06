#include <string.h>

#include "cfg_production.h"
#include "utils.h"

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
	list_rhs* it_list = prod->rhs_element;
	while (it_list) {
		list_rhs* next = it_list->next;
		FREE(it_list);
		it_list = next;
	}
	FREE(prod);
}

int
add_symbol_rhs(production_t* prod, symbol_t const* symbol) {
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


