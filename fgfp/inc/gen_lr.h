#ifndef GEN_LR_H
#define GEN_LR_H

#include "fgfp.h"
#include "gen_cfg.h"
#include "vector.h"

#define SHIFT_STR	"SHIFT"
#define REDUCE_STR	"REDUCE"
#define GOTO_STR	"GOTO"
#define ACCEPT_STR	"ACCEPT"

void gen_action_enum(int);
void gen_lr_useful_macro(int);
void gen_action_table(int, cfg_t const*, vector_t const*, char const*);
void gen_goto_table(int, cfg_t const*, vector_t const*, char const*);

#endif /* GEN_LR_H */
