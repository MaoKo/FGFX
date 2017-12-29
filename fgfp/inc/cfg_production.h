#ifndef CFG_PRODUCTION_H
#define CFG_PRODUCTION_H

#include <stdbool.h>
#include "fgfp.h"

#define LHS(prod)	((prod)->symbol_lhs)
#define RHS(prod)	((prod)->rhs_element)

production_t* new_production(symbol_t*, size_t);
void del_production(production_t*);
int add_symbol_rhs(production_t*, symbol_t*);
symbol_t* last_symbol_in_prod(production_t const*);
int preprocess_literal(cfg_t const*);
bool list_is_nullable(list_rhs_t const*);
bool production_is_nullable(production_t const*);
bitset_t* first_list_rhs_t(list_rhs_t const*);
bitset_t* first_production(production_t const*);
list_rhs_t const* match_symbol_production(list_rhs_t const*, symbol_t const*);
vector_t* stack_production_lhs(cfg_t const*, symbol_t*);
int unreachable_production(cfg_t const*);
bool disjoint_select_set(cfg_t const*, symbol_t*);

#endif /* CFG_PRODUCTION_H */
