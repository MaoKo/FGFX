#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdint.h>

#include "buffer.h"

#define FILDE_LEX(lex)		((lex)->filde)
#define LAST_TOKEN(lex)		((lex)->last_token)
#define LAST_LEXEME(lex)	((lex)->last_lexeme)
#define C_LEXEME(lex)		(BODY_BUFFER(LAST_LEXEME(lex)))
#define CURRENT_LINE(lex)	((lex)->lineno)

#define NO_TOKEN			(-1)
#define START_LINE			(1)

typedef struct {
	int filde;
	bool carry_nl;
	int last_token;
	buffer_t* push_back;
	buffer_t* last_lexeme;
	int lineno;
} lexer_t;

lexer_t* new_lexer(int);
void del_lexer(lexer_t*);
long advance_token(lexer_t*);
int peek_token(lexer_t*);
bool in_first(lexer_t*, ...);

#endif /* LEXER_H */
