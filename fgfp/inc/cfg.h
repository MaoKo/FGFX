#ifndef CFG_H
#define CFG_H

#include "fgfp.h"

#define _ONLY_TOKEN_
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

#define NON_TERMINAL			T_NTER

#define IS_TERMINAL(symbol)		((symbol)->kind == T_TERMINAL)
#define IS_NON_TERMINAL(symbol)	((symbol)->kind == NON_TERMINAL)
#define IS_LITERAL(symbol)		((symbol)->kind == T_LITERAL)

#define IS_EOF(symbol)			((symbol)->is_eof)
#define NOT_PREC				(-1)

#define PRECEDENCE(sym)			((!(sym)->prec) ? NOT_PREC :\
												(sym)->prec->precedence)

#define RIGHT(sym)				((!(sym)->prec) ? false : (sym)->prec->right)
#define LEFT(sym)				((!(sym)->prec) ? false : (sym)->prec->left)

#define SET_START(cfg, index)	((cfg)->goal = index)

cfg_t* new_cfg(int);
void del_cfg(cfg_t*);
cfg_t* parse_cfg(int);
void augment_grammar(cfg_t*);
int cfg_sanity_check(cfg_t*);
#ifdef PRINT_DEBUG
void print_terminal(cfg_t const*);
void print_non_terminal(cfg_t const*);
void print_production(cfg_t const*);
void print_nullable(cfg_t const*);
void print_first_set(cfg_t const*);
void print_follow_set(cfg_t const*);
#endif /* PRINT_DEBUG */

#endif /* CFG_H */
