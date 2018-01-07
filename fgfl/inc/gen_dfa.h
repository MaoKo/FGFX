#ifndef GEN_DFA_H
#define GEN_DFA_H

#include "fgfl.h"
#include "vector.h"

#define STATE_TOKEN_STR			"_ONLY_STATE_TOKEN_"

#define IFNDEF_ONLY_STATE_TOKEN(x)	dprintf(x, "#ifndef " STATE_TOKEN_STR NL NL)
#define ENDIF_ONLY_STATE_TOKEN(x)	dprintf(x, "#endif /* "\
											STATE_TOKEN_STR " */" NL)

#define DFA_START_STATE(x)		dprintf(x, DEFINE(START_STATE, 1) NL)
#define DFA_DEAD_STATE(x)		dprintf(x, DEFINE(DEAD_STATE, 0) NL NL)

#define DFA_MACRO(x)			(DFA_START_STATE(x), DFA_DEAD_STATE(x))

#define ENUM_LINE(f, x, p)		dprintf(f, TAB p SEP "%s" COMMA NL, x)

#define ENUM_TOKEN_LINE(f, x)	ENUM_LINE(f, x, TOKEN_PREFIX)
#define ENUM_STATE_LINE(f, x)	ENUM_LINE(f, x, STATE_PREFIX)

#define TOKEN_PREFIX			"T"
#define STATE_PREFIX			"S"

void gen_state_enum(int, lexical_spec_t const*);
void gen_token_enum(int, lexical_spec_t const*);
void gen_dfa_typedef(int, vector_t const*,
							vector_t const*, spec_entry_t const*);
void gen_state_table(int, vector_t const*, char const*, spec_entry_t const*);
void gen_final_table(int, vector_t const*, char const*, spec_entry_t const*);
void gen_skip_table(int, lexical_spec_t const* spec, char const*);
void gen_change_state(int, char const*, lexical_spec_t*);

#endif /* GEN_DFA_H */
