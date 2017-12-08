#ifndef OUTPUT_LL_H
#define OUTPUT_LL_H

#include "cfg.h"
#include "vector.h"

void output_location_token(int, char const*);
void output_nter_symbol(int, char const*);
void output_non_terminal_enum(int, vector_t const*);
void output_ll_table(int, cfg_t const*, vector_t const*);

#endif /* OUTPUT_LL_H */
