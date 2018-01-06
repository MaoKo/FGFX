#ifndef GEN_DFA_H
#define GEN_DFA_H

#include "fgfl.h"
#include "vector.h"

#define IFNDEF_ONLY_TOKEN(x)	dprintf(x, "#ifndef _ONLY_TOKEN_" NL NL)
#define ENDIF_ONLY_TOKEN(x)		dprintf(x, "#endif /* _ONLY_TOKEN_ */" NL)

#define ENUM_LINE(f, x, p)		dprintf(f, TAB p SEP "%s" COMMA NL, x)

#define ENUM_TOKEN_LINE(f, x)	ENUM_LINE(f, x, TOKEN_PREFIX)
#define ENUM_STATE_LINE(f, x)	ENUM_LINE(f, x, STATE_PREFIX)

#define TOKEN_PREFIX			"T"
#define STATE_PREFIX			"S"

void gen_dfa_typedef(int, size_t, size_t);
void gen_state_enum(int, lexical_spec_t const*);
void gen_token_enum(int, lexical_spec_t const*);
void gen_state_table(int, vector_t const*, char const*);
void gen_final_table(int, vector_t const*, char const*);
void gen_skip_table(int, lexical_spec_t const* spec, char const*);
void gen_dfa_useful_macro(int);

#endif /* GEN_DFA_H */
