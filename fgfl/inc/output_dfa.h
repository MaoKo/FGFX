#ifndef OUTPUT_DFA_H
#define OUTPUT_DFA_H

#include "vector.h"
#define IFNDEF_ONLY_TOKEN(x)	dprintf(x, "#ifndef ONLY_TOKEN\n\n")
#define ENDIF_ONLY_TOKEN(x)	dprintf(x, "#endif /* ONLY_TOKEN */\n")

void output_state_table(int, vector_t const*, char const*);
void output_final_table(int, vector_t const*, char const*);
void output_skip_table(int, vector_t const*, char const*);
int output_dfa_matrix(char const*, vector_t const*,
				vector_t const*, vector_t const*);

#endif /* OUTPUT_DFA_H */
