#if 0

#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

extern char const* beg_lexeme;
extern size_t size_lexeme;

int advance_token(void);
int peek_token(void);
void set_stream(char const*);

#endif /* LEXER_H */

#endif /* 0 */
