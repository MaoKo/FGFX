#if 0

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

extern char const* stream;
extern char const* beg_lexeme;

extern size_t size_lexeme;

#define SET_STREAM(str)     (stream = (str))

int advance_token(void);
int peek_token(void);

#endif /* LEXER_H */

#endif /* 0 */
