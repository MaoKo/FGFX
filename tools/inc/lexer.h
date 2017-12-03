#ifndef LEXER_H
#define LEXER_H

#include "fgfl.h"

#define FILDE_LEX(lex)		((lex)->filde)
#define LAST_TOKEN(lex)		((lex)->last_token)
#define LAST_LEXEME(lex)	((lex)->last_lexeme)
#define CURRENT_LINE(lex)	((lex)->lineno)

lexer_t* new_lexer(int);
void del_lexer(lexer_t*);
int advance_token(lexer_t*);
int peek_token(lexer_t*);
int is_final_state(int);

#endif /* LEXER_H */
