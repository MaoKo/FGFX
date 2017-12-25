#ifndef DISPLAY_DFA_H
#define DISPLAY_DFA_H

#include "vector.h"

#define IFNDEF_ONLY_TOKEN(x)	dprintf(x, "#ifndef _ONLY_TOKEN_" NL NL)
#define ENDIF_ONLY_TOKEN(x)		dprintf(x, "#endif /* _ONLY_TOKEN_ */" NL)

#define TOKEN_PREFIX		"T"

void display_state_table(int, vector_t const*, char const*);
void display_final_table(int, vector_t const*, char const*);
void display_skip_table(int, vector_t const*, char const*);
int display_dfa_matrix(char const*, vector_t const*,
				vector_t const*, vector_t const*);

#endif /* DISPLAY_DFA_H */
