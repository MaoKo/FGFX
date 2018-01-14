#ifndef LEXICAL_SPEC_H
#define LEXICAL_SPEC_H

#include "fgfl.h"

#define	_ONLY_STATE_TOKEN_
#include "fgfx.lex.h"
#undef	_ONLY_STATE_TOKEN_

#define KIND_IN2_STR(x)			((x == T_TERMINAL) ? "TERMINAL" :\
									((x == T_KEYWORD) ? "KEYWORD" : "STATE"))

#define REGEX_LST_SECT(x)		((x == T_TOKEN) ?  "$TOKEN" : "$SKIP")
#define STATE_TOKEN_LST_SECT(x)	((x == T_STATE) ?  "$STATE" : "$KEYWORD")

#define USED_ENTRY(entry)		(entry->is_used = true)
#define NONE_BEGIN				(-1)

int cmp_token_entry(spec_entry_t*, char const*);
lexical_spec_t* parse_lexical_spec(int);
void del_lexical_spec(lexical_spec_t*);
int spec_sanity_check(lexical_spec_t*);
#ifdef PRINT_DEBUG
void print_token_entry(lexical_spec_t*);
#endif /* PRINT_DEBUG */

#endif /* LEXICAL_SPEC_H */
