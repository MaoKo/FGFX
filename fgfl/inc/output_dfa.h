#ifndef OUTPUT_DFA_H
#define OUTPUT_DFA_H

#include "vector.h"
#define IFNDEF_ONLY_TOKEN	dprintf(filde, "#ifndef ONLY_TOKEN\n\n")
#define ENDIF_ONLY_TOKEN	dprintf(filde, "#endif /* ONLY_TOKEN */\n")

int output_matrix(char const*, vector_t const*, vector_t const*, vector_t const*);

#endif /* OUTPUT_DFA_H */
