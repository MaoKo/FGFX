#ifndef TOKENDEF_H
#define TOKENDEF_H

#include "fgfl.h"

#include "regex.h"
#include "nfa.h"

#define ENTRY_COM_SYM		'#'
#define ENTRY_LOCAL_SYM		'@'
#define ENTRY_EQUAL_SYM		'='

#define READ			1
#define NO_READ			0

void del_token_spec(token_spec_t*);
int retreive_token_entry(char const*, token_spec_t**);
int token_spec2nfa(token_spec_t*);

#endif /* TOKENDEF_H */
