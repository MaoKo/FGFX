#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdint.h>

#include "buffer.h"
#include "vector.h"

#define FILDE_LEX(lex)      ((lex)->filde)
#define LAST_TOKEN(lex)     ((lex)->last_token)
#define LAST_LEXEME(lex)    ((lex)->last_lexeme)
#define C_LEXEME(lex)       (BODY_BUFFER(LAST_LEXEME(lex)))
#define CURRENT_LINE(lex)   ((lex)->lineno)

#define NO_TOKEN            (-1)
#define NO_CHAR             (-1)
#define START_LINE          (1)

typedef struct {
    int filde;
    int last_token;
    buffer_t* push_back;
    buffer_t* last_lexeme;
    vector_t* stack_state;
    size_t crt_state;
    int lineno;
} lexer_t;

lexer_t* new_lexer(int);
void del_lexer(lexer_t*);
bool check_present_table(int8_t*, int);
long advance_token(lexer_t*);
int peek_token(lexer_t*);
bool in_first(lexer_t*, ...);

#endif /* LEXER_H */
