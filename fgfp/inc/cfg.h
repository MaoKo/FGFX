#ifndef CFG_H
#define CFG_H

#include "fgfp.h"

#define _ONLY_TOKEN_
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

#define TERMINAL				T_GLOBAL_TOK
#define NON_TERMINAL			T_NTER
#define LITERAL					T_LITERAL

#define IS_TERMINAL(symbol)		((symbol)->kind == TERMINAL)
#define IS_NON_TERMINAL(symbol)	((symbol)->kind == NON_TERMINAL)
#define IS_LITERAL(symbol)		((symbol)->kind == LITERAL)
#define IS_EOF(symbol)			((symbol)->is_eof)
#define GET_INDEX(symbol)		((symbol)->index)
#define NOT_PREC				(-1)

#define PRECEDENCE(sym)			((sym)->prec->precedence)
#define RIGHT(sym)				((sym)->prec->right)
#define LEFT(sym)				((sym)->prec->left)

#define SET_START(cfg, index)	((cfg)->goal = index)

cfg_t* new_cfg(int);
void del_cfg(cfg_t*);
cfg_t* parse_cfg(int);
void augment_grammar(cfg_t*);
int detect_bad_symbol(cfg_t*);
#ifdef PRINT_DEBUG
void print_terminal(cfg_t const*);
void print_non_terminal(cfg_t const*);
void print_production(cfg_t const*);
void print_nullable(cfg_t const*);
#endif /* PRINT_DEBUG */

#endif /* CFG_H */
