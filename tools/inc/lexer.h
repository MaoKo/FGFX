#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include "buffer.h"

#define FILDE_LEX(lex)		((lex)->filde)
#define LAST_TOKEN(lex)		((lex)->last_token)
#define LAST_LEXEME(lex)	((lex)->last_lexeme)
#define C_LEXEME(lex)		(BODY_BUFFER(LAST_LEXEME(lex)))
#define CURRENT_LINE(lex)	((lex)->lineno)

typedef struct {
	int filde;
	int last_char;
	int last_token;
	buffer_t* last_lexeme;
	int lineno;
} lexer_t;

lexer_t* new_lexer(int);
void del_lexer(lexer_t*);
long advance_token(lexer_t*);
int peek_token(lexer_t*);
bool in_first(lexer_t*, ...);
size_t is_final_state(int);

#endif /* LEXER_H */
