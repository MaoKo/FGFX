#ifndef TOKEN_DEF_H
#define TOKEN_DEF_H

#include "fgfl.h"

#define	_ONLY_TOKEN_
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

#define GLOBAL				T_GLOBAL_TOK
#define LOCAL				T_LOCAL_TOK
#define KEYWORD				T_KEYWORD
#define USED_ENTRY(entry)	(entry->used = true)

token_spec_t* parse_token_def(int);
void del_token_spec(token_spec_t*);

#endif /* TOKEN_DEF_H */
