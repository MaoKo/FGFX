#ifndef DISPLAY_LR_H
#define DISPLAY_LR_H

#include "display_cfg.h"

#define SHIFT_STR	"SHIFT"
#define REDUCE_STR	"REDUCE"
#define GOTO_STR	"GOTO"
#define ACCEPT_STR	"ACCEPT"

void display_action_enum(int);
void display_lr_useful_macro(int);
void display_action_table(int, cfg_t const*, vector_t const*);
void display_goto_table(int, cfg_t const*, vector_t const*);

#endif /* DISPLAY_LR_H */
