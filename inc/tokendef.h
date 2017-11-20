#ifndef TOKENDEF_H
#define TOKENDEF_H

#include "fgfl.h"

#include "regex.h"
#include "nfa.h"
#include "buffer.h"

#define READ			1
#define NO_READ			0

#define USED_ENTRY(entry)	(entry->used = true)

void del_token_spec(token_spec_t*);
int Reggen(char const*, token_spec_t**);

#endif /* TOKENDEF_H */
