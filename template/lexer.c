#if 0

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "lexer.h"
#include "template.lex.h" // insert the output of FGFL

char const* stream = NULL;
char const* beg_lexeme = NULL;

#ifndef LOOK_TABLE_NOT_DEFINE
bool unget_input = false;
char const* backup_str = NULL;
#endif /* LOOK_TABLE_NOT_DEFINE */

size_t size_lexeme = 0;

int peek = -1;

#if !defined(SKIP_TABLE_NOT_DEFINE) || !defined(LOOK_TABLE_NOT_DEFINE)
static inline bool
check_present_table(int8_t* table, int token) {
    for (size_t i = 0; table[i] != -1; ++i) {
        if (token == table[i])
            { return (true); }
    }
    return (false);
}
#endif

static int
get_next_token(void) {
    int last_match = T_ERROR;
    if (peek != -1) {
        last_match = peek;
        peek = -1;
        return (last_match);
    }
    if (!*stream)
        { return (T_EOF); }
    int state = START_STATE;
    beg_lexeme = stream;
    while (*stream && (state != DEAD_STATE)) {
        state = /* NAME PREFIX */state_table[state][(int)*stream];
        if (state != DEAD_STATE)
            { break; }

        ++stream;

#ifndef LOOK_TABLE_NOT_DEFINE
        if (/* NAME PREFIX */middle_table[state] && !unget_input) {
            unget_input = true;
            backup_str = stream;
        }
#endif /* LOOK_TABLE_NOT_DEFINE */

        size_t i = 0;
        while (* /* NAME PREFIX */final_table[i]) {
            if (* /* NAME PREFIX */final_table[i] == state)
                { last_match = /* NAME PREFIX */final_table[i][1]; }
            ++i;        
        }
    }
#ifndef LOOK_TABLE_NOT_DEFINE
    if (unget_input &&
                check_present_table(/* NAME PREFIX */look_table, last_match)) {
        stream = backup_str;
        unget_input = false;
    }
#endif /* LOOK_TABLE_NOT_DEFINE */

    size_lexeme = (stream - beg_lexeme);
    return (last_match);
}

int
advance_token(void) {
    if (!stream)
        { return (-1); }
    int token;
#ifndef SKIP_TABLE_NOT_DEFINE
    do {
#endif /* SKIP_TABLE_NOT_DEFINE */
        token = get_next_token();
#ifndef SKIP_TABLE_NOT_DEFINE
    } while (check_present_table(/* NAME PREFIX */skip_table, token));
#endif /* SKIP_TABLE_NOT_DEFINE */
    return (token);
}

int
peek_token(void) {
    if (peek == -1)
        { peek = advance_token(); }
    return (peek);
}

void
set_stream(char const* str) {
    stream = str;
}

#endif /* 0 */
