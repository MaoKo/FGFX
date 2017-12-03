#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "lexer.h"
#include "fgfl.lex.h"
#include "utils.h"
#include "buffer.h"

lexer_t*
new_lexer(int filde) {
	lexer_t* lex = NEW(lexer_t, 1);
	if (!lex)
		{ return (NULL); }
	memset(lex, 0, sizeof(lexer_t));
	lex->filde = filde;
	lex->last_char = -1;
	return (lex);
}

void
del_lexer(lexer_t* lex) {
	if (lex)
		{ del_buffer(lex->last_lexeme); }
	FREE(lex);
}

static int
get_next_token(lexer_t* lex) {
	int rd, state = START_STATE, last_match = TNONE;
	int c = 0;

	if (!lex->last_lexeme)
		{ lex->last_lexeme = new_buffer(); }
	if (lex->last_char != -1) {
		state = fgfl_state_table[state][lex->last_char];
		last_match = is_final_state(state);
		write_char_buffer(lex->last_lexeme, lex->last_char);
		if (lex->last_char == '\n')
			{ ++(lex->lineno); }
	}

	int first_read = true;
	while (state != DEAD_STATE) {
		rd = read(lex->filde, &c, 1);
		if (!rd) {
			if (first_read)
				{ return (TEOF); }
			break;
		}
		state = fgfl_state_table[state][c];
		if (is_final_state(state))
			{ last_match = is_final_state(state); }
		write_char_buffer(lex->last_lexeme, c);
		first_read = false;
		if (state != DEAD_STATE && c == '\n')
			{ ++(lex->lineno); }
	}
	
	if (last_match == TNONE) {
		fprintf(stderr, "Lexical Error <%d:%s>.\n",
			lex->lineno, BODY_BUFFER(lex->last_lexeme));
		lex->last_char = -1;
	}
	else {
		lex->last_char = c;
		unget_c_buffer(lex->last_lexeme, 1);
	}
	return (last_match);
}

static int
is_present_va(int value, va_list args) {
	int i;
	while ((i = va_arg(args, int)) != -1) {
		if (value == i)
			{ return (true); }
	}
	return (false);
}

int
advance_token(lexer_t* lex, ...) {
	if (!lex)
		{ return (-1); }
	va_list args, cpy;
	va_start(args, lex);

	reset_buffer(lex->last_lexeme);
	while (1) {
		lex->last_token = get_next_token(lex);
		bool is_p = false;
		va_copy(cpy, args);
		if (is_present_va(lex->last_token, cpy)) {
			reset_buffer(lex->last_lexeme);
			is_p = true;
		}
		va_end(cpy);
		if (!is_p)
			{ break; }
	}
	va_end(args);
	return (lex->last_token);
}

int
peek_token(lexer_t* lex) {
	if (!lex)
		{ return (-1); }
	if (lex->last_token == -1)
		{ lex->last_token = advance_token(lex); }
	return (lex->last_token);
}

int
is_final_state(int state) {
	for (size_t i = 0; i < SIZE_FINAL_TAB; ++i) {
		if (fgfl_final_table[i][0] == state)
			{ return (fgfl_final_table[i][1]); }
	}
	return (TNONE);
}

