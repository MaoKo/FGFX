#ifndef CFG_PRODUCTION_H
#define CFG_PRODUCTION_H

#include "fgfp.h"

production_t* new_production(symbol_t*);
void del_production(production_t*);
int add_symbol_rhs(production_t*, symbol_t const*);

#endif /* CFG_PRODUCTION_H */
