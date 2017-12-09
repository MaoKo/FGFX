#ifndef CFG_H
#define CFG_H

#include "fgfp.h"

#define ONLY_TOKEN
#include "fgfx.lex.h"
#undef ONLY_TOKEN

#define TERMINAL		TGLOBAL_TOK
#define NON_TERMINAL		TNTER
#define ALIAS			TLITERAL

#define IS_TERMINAL(symbol)	((symbol)->kind == TERMINAL)
#define IS_NON_TERMINAL(symbol)	((symbol)->kind == NON_TERMINAL)
#define SET_START(cfg, index)	((cfg)->goal = index)

cfg_t* new_cfg(void);
void del_cfg(cfg_t*);
cfg_t* parse_cfg(int);
void augment_grammar(cfg_t*);
int detect_bad_symbol(cfg_t*);

#endif /* CFG_H */
