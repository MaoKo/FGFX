#ifndef OUTPUT_LL_H
#define OUTPUT_LL_H

#include "cfg.h"
#include "vector.h"

#define NTER_PREFIX	"N_"

void output_location_token(int, vector_t const*);
void output_nter_symbol(int, vector_t const*, size_t);
void output_non_terminal_enum(int, vector_t const*);
void output_ll_useful_macro(int, cfg_t const*);
void output_ll_table(int, cfg_t const*, vector_t const*, char const*);
void output_synchronizing_token(int, cfg_t const*, char const*);
int output_ll_matrix(cfg_t const*, char const*);

#endif /* OUTPUT_LL_H */
