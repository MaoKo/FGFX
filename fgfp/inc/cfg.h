#ifndef CFG_H
#define CFG_H

#include "fgfp.h"

#define SET_START(cfg, index)	((cfg)->goal = index)

cfg_t* new_cfg(void);
void del_cfg(cfg_t*);
cfg_t* parse_cfg(int);

#endif /* CFG_H */
