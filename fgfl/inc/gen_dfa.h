#ifndef GEN_DFA_H
#define GEN_DFA_H

#include "vector.h"

#define IFNDEF_ONLY_TOKEN(x)	dprintf(x, "#ifndef _ONLY_TOKEN_" NL NL)
#define ENDIF_ONLY_TOKEN(x)		dprintf(x, "#endif /* _ONLY_TOKEN_ */" NL)

#define ENUM_LINE(f, x)			dprintf(f, TAB TOKEN_PREFIX\
												SEP "%s" COMMA NL, x)

#define TOKEN_PREFIX			"T"

void gen_state_table(int, vector_t const*, char const*);
void gen_final_table(int, vector_t const*, char const*);
void gen_skip_table(int, vector_t const*, char const*);
int gen_dfa_matrix(char const*, vector_t const*,
									vector_t const*, vector_t const*);

#endif /* GEN_DFA_H */
