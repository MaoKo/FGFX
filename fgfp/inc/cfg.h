#ifndef CFG_H
#define CFG_H

#include "fgfp.h"

#define IS_TERMINAL(symbol)	((symbol)->kind == TTOKEN)
#define IS_NON_TERMINAL(symbol)	((symbol)->kind == TNON_TER)
#define SET_START(cfg, index)	((cfg)->goal = index)

cfg_t* new_cfg(void);
void del_cfg(cfg_t*);
cfg_t* parse_cfg(int);
void augment_grammar(cfg_t*);
int detect_bad_symbol(cfg_t*);
void detect_nullable(cfg_t*);
void compute_first(cfg_t*);
void compute_follow(cfg_t*);

#endif /* CFG_H */
