#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>
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
#define REG_DQUOTE		'"'
#define REG_HYPHEN		'-'
#define REG_SEMI		';'

#define ALONE_S			true
#define MULTI_S			false

regex_node_t* regex_to_ast(token_spec_t*, char const*);
void del_regex_node(regex_node_t*);

#endif /* REGEX_H */
