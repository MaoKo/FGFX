#ifndef REGEX_H
#define REGEX_H

#include "tokendef.h"

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
#define REG_DQUOTE		'"'
#define REG_HYPHEN		'-'
#define REG_SEMI		';'

#define ALONE_S			true
#define MULTI_S			false

node_ast_t* regex2ast(token_spec_t*);
void del_node_ast(node_ast_t*);

#endif /* REGEX_H */
