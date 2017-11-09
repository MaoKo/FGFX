#ifndef REGEX_H
#define REGEX_H

#include "fgfl.h"

#define REG_ESCAPE		'\\'
#define REG_UNION		'|'
#define REG_LPAREN		'('
#define REG_RPAREN		')'
#define REG_LBRACK		'['
#define REG_RBRACK		']'
#define REG_KLEENE		'*'
#define REG_PKLEENE		'+'
#define REG_OKLEENE		'?'
#define REG_LBRACE		'{'
#define REG_RBRACE		'}'
#define REG_COMMA		','
#define REG_CARET		'^'
#define REG_DOT			'.'
#define DQUOTE			'"'

#define ALONE_S			true
#define MULTI_S			false

node_ast_t* regex2ast(char const*, vector_t const*);
void del_node_ast(node_ast_t*);

#endif /* REGEX_H */
