#ifndef CFG_PRODUCTION_H
#define CFG_PRODUCTION_H

#include <stdbool.h>
#include "fgfp.h"

production_t* new_production(symbol_t*);
void del_production(production_t*);
int add_symbol_rhs(production_t*, symbol_t const*);
bool production_is_nullable(production_t const*);
bitset_t* first_production(cfg_t const*, production_t const*);

#endif /* CFG_PRODUCTION_H */
