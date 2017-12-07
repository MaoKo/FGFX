#ifndef LL_H
#define LL_H

#include <stdbool.h>

#include "fgfp.h"
#include "vector.h"

bool is_ll1(cfg_t const*);
vector_t* gen_ll1_table(cfg_t const*);

#endif /* LL_H */
