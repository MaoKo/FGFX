#ifndef DISPLAY_CFG_H
#define DISPLAY_CFG_H

#include "cfg.h"
#include "vector.h"

#define PRIME_STR	"_PRIME"

#define NTER_PREFIX	"N"
#define PROD_PREFIX	"P"

void display_location_token(int, vector_t const*);
void display_nter_symbol(int, cfg_t const*, size_t, bool);
void display_non_terminal_enum(int, cfg_t const*);
void display_nproduction_macro(int, cfg_t const*, size_t);
void display_production_macro(int, cfg_t const*);
void display_synchronizing_token(int, cfg_t const*, char const*);
int display_parsing_table(cfg_t const*, char const*);

#endif /* DISPLAY_CFG_H */
