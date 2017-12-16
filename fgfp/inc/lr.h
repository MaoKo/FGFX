#ifndef LR_H
#define LR_H

#include "fgfp.h"
#include "bitset.h"

int new_item(production_t const*);
void del_record_item(void);
bitset_t* closure(cfg_t const*, bitset_t*);

#endif /* LR_H */
