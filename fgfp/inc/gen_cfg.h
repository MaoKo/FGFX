#ifndef GEN_CFG_H
#define GEN_CFG_H

#include "cfg.h"
#include "vector.h"

#define PRIME_STR	"_PRIME"

#define OPT_STR		"OPT"
#define LST_STR		"LST"
#define GRP_STR		"GRP"

#define NTER_PREFIX	"N"
#define PROD_PREFIX	"P"

#define LHS_STR		"LHS"
#define RHS_STR		"RHS"

void gen_location_token(int, vector_t const*);
void gen_nter_symbol(int, cfg_t const*, size_t, bool);
void gen_non_terminal_enum(int, cfg_t const*);
void gen_rhs_table(int, cfg_t const*, char const*);
void gen_lhs_table(int, cfg_t const*, char const*);
void gen_nproduction(int, cfg_t const*, size_t);
void gen_production_enum(int, cfg_t const*);
void gen_synchronizing_token(int, cfg_t const*, char const*);

#endif /* GEN_CFG_H */
