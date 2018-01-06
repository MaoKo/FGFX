#ifndef LEXICAL_SPEC_H
#define LEXICAL_SPEC_H

#include "fgfl.h"

#define	_ONLY_TOKEN_
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

#define GLOBAL				T_GLOBAL_TOK
#define LOCAL				T_LOCAL_TOK
#define KEYWORD				T_KEYWORD
#define STATE				T_STATE

#define REGEX_LST_SECT(x)	((x == T_TOKEN) ?  "$TOKEN" : "$SKIP")
#define TOKEN_LST_SECT(x)	((x == T_STATE) ?  "$STATE" : "$KEYWORD")

#define USED_ENTRY(entry)	(entry->is_used = true)

lexical_spec_t* parse_lexical_spec(int);
void del_lexical_spec(lexical_spec_t*);
int spec_sanity_check(lexical_spec_t*);
#ifdef PRINT_DEBUG
void print_token_entry(lexical_spec_t*);
#endif /* PRINT_DEBUG */

#endif /* LEXICAL_SPEC_H */
