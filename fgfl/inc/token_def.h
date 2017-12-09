#ifndef TOKEN_DEF_H
#define TOKEN_DEF_H

#include "fgfl.h"

#define USED_ENTRY(entry)	(entry->used = true)
#define REDEFINNING		(-2)

void del_token_spec(token_spec_t*);
int regex_gen(char const*, token_spec_t**);

#endif /* TOKEN_DEF_H */
