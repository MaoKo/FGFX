#if 0

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "lexer.h"
#include "template.lex.h" // insert the output of FGFL

char const* stream = NULL;
char const* beg_lexeme = NULL;

int peek = -1;

static int
get_next_token(void) {
	int last_match = T_ERROR;
	if (peek != -1) {
		last_match = peek;
		peek = -1;
		return (last_match);
	}
	if (!*stream)
		{ return (T_EOF); }
	int state = START_STATE;
	beg_lexeme = stream;
	while (*stream && (state != DEAD_STATE)) {
		state = /* NAME PREFIX */state_table[state][(int)*stream];
		size_t i = 0;
		while (* /* NAME PREFIX */final_table[i]) {
			if (* /* NAME PREFIX */final_table[i] == state)
				{ last_match = /* NAME PREFIX */final_table[i][1]; }
			++i;		
		}
		if (state != DEAD_STATE)
			{ ++stream; }
	}
	return (last_match);
}

#ifndef SKIP_TABLE_NOT_DEFINE
static inline bool
allow_skip(int token) {
	for (size_t i = 0; /* NAME PREFIX */skip_table[i] != -1; ++i) {
		if (token == /* NAME PREFIX */skip_table[i])
			{ return (true); }
	}
	return (false);
}
#endif /* SKIP_TABLE_NOT_DEFINE */

int
advance_token(void) {
	if (!stream)
		{ return (-1); }
	int token;
#ifndef SKIP_TABLE_NOT_DEFINE
	do {
#endif /* SKIP_TABLE_NOT_DEFINE */
	token = get_next_token();
#ifndef SKIP_TABLE_NOT_DEFINE
	} while (allow_skip(token));
#endif /* SKIP_TABLE_NOT_DEFINE */
	return (token);
}

int
peek_token(void) {
	if (peek == -1)
		{ peek = advance_token(); }
	return (peek);
}

void
set_stream(char const* str) {
	stream = str;
}

#endif /* 0 */
