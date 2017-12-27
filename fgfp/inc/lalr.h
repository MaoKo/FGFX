#ifndef LALR_H
#define LALR_H

#include "vector.h"
#include "bitset.h"
#include "fgfp.h"

bitset_t* mark_equiv_state(vector_t const*);
vector_t* gen_lalr1_states(cfg_t const*);

#endif /* LALR_H */
