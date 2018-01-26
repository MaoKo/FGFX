#ifndef LEXICAL_SPEC_H
#define LEXICAL_SPEC_H

#include "fgfl.h"

#define _ONLY_STATE_TOKEN_
#include "fgfx.lex.h"
#undef  _ONLY_STATE_TOKEN_

#define KIND_IN2_STR(x)         ((x == T_TERMINAL) ? "TERMINAL" :\
                                    ((x == T_KEYWORD) ? "KEYWORD" : "STATE"))

#define REGEX_LST_SECT(x)       ((x == T_TOKEN) ?  "$TOKEN" : "$SKIP")
#define STATE_TYPE_LST_SECT(x)  ((x == T_STATE) ?  "$STATE" : "$KEYWORD")

#define KIND_IN2_ACTION(x)      ((x == T_BEGIN) ? "$BEGIN" :\
                                    (x == T_PUSH) ? "$PUSH" : "$POP")

#define USED_ENTRY(entry)       (entry->is_used = true)

enum {
    _BEGIN  = 0x1000,
    _PUSH   = 0x2000,
    _POP    = 0x4000,
};

#define BEGIN(x)                (x | _BEGIN)
#define PUSH(x)                 (x | _PUSH)
#define POP                     (_POP)

#define NONE_STATE              (-1)
#define NONE_ACTION             (-1)

int cmp_token_entry(spec_entry_t*, char const*);
lexical_spec_t* parse_lexical_spec(int);
void del_lexical_spec(lexical_spec_t*);
int spec_sanity_check(lexical_spec_t*);
#ifdef PRINT_DEBUG
void print_token_entry(lexical_spec_t*);
#endif /* PRINT_DEBUG */

#endif /* LEXICAL_SPEC_H */
