#ifndef CFG_PRODUCTION_H
#define CFG_PRODUCTION_H

#include <stdbool.h>
#include "fgfp.h"

production_t* new_production(symbol_t*, size_t);
void del_production(production_t*);
int add_symbol_rhs(production_t*, symbol_t*);
int preprocess_literal(cfg_t const*);
bool production_is_nullable(production_t const*);
bitset_t* first_production(production_t const*);
list_rhs const* match_symbol_production(list_rhs const*, symbol_t const*);
vector_t* stack_production_lhs(cfg_t const*, symbol_t*);
int unreachable_production(cfg_t const*);
bool disjoint_select_set(cfg_t const*, symbol_t*);

#endif /* CFG_PRODUCTION_H */
