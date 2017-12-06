#ifndef CFG_SET_OP_H
#define CFG_SET_OP_H

#include "fgfp.h"

void detect_nullable(cfg_t*);
void compute_first(cfg_t*);
void compute_follow(cfg_t*);

#endif /* CFG_SET_OP_H */
