#ifndef DISPLAY_DFA_H
#define DISPLAY_DFA_H

#include "vector.h"

#define IFNDEF_ONLY_TOKEN(x)	dprintf(x, "#ifndef ONLY_TOKEN\n\n")
#define ENDIF_ONLY_TOKEN(x)	dprintf(x, "#endif /* ONLY_TOKEN */\n")

#define SEP					"_"
#define TOKEN_PREFIX		"T"

void display_state_table(int, vector_t const*, char const*);
void display_final_table(int, vector_t const*, char const*);
void display_skip_table(int, vector_t const*, char const*);
int display_dfa_matrix(char const*, vector_t const*,
				vector_t const*, vector_t const*);

#endif /* DISPLAY_DFA_H */
