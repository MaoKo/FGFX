#ifndef DISPLAY_LL_H
#define DISPLAY_LL_H

#include "display_cfg.h"

void display_ll_useful_macro(int, cfg_t const*);
void display_ll_table(int, cfg_t const*, vector_t const*, char const*);
void display_synchronizing_token(int, cfg_t const*, char const*);
int display_ll_matrix(cfg_t const*, char const*);

#endif /* DISPLAY_LL_H */
