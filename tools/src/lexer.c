#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "lexer.h"
#include "fgfx.lex.h"
#include "utils.h"
#include "buffer.h"

lexer_t*
new_lexer(int filde) {
	lexer_t* lex = NEW(lexer_t, 1);
	if (!lex)
		{ return (NULL); }
	memset(lex, 0, sizeof(lexer_t));
	lex->filde = filde;
	lex->last_token = lex->last_char = -1;
	lex->lineno = 1;
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
	int rd, state = START_STATE, last_match = T_ERROR;
	int c = 0;

	if (!lex->last_lexeme)
		{ lex->last_lexeme = new_buffer(); }
	if (lex->last_char != -1) {
		state = fgfx_state_table[state][lex->last_char];
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
				{ return (T_EOF); }
			break;
		}
		state = fgfx_state_table[state][c];
		if (is_final_state(state) != T_ERROR)
			{ last_match = is_final_state(state); }
		write_char_buffer(lex->last_lexeme, c);
		first_read = false;
		if (state != DEAD_STATE && c == '\n')
			{ ++(lex->lineno); }
	}
	
	if (last_match == T_ERROR) {
		fprintf(stderr, "Lexical Error <%d:%s>.\n",
			lex->lineno, C_LEXEME(lex));
		lex->last_char = -1;
	}
	else {
		lex->last_char = c;
		unget_c_buffer(lex->last_lexeme, 1);
	}
	return (last_match);
}

static int
allow_skip(int token) {
	for (size_t i = 0; fgfx_skip_table[i] != -1; ++i) {
		if (token == fgfx_skip_table[i])
			{ return (true); }
	}
	return (false);
}

long
advance_token(lexer_t* lex) {
	if (!lex)
		{ return (-1); }
	long found_token;
	if (lex->last_token != -1)
		{ found_token = lex->last_token; }
	else {
		reset_buffer(lex->last_lexeme);
		while (1) {
			found_token = get_next_token(lex);
			if (!allow_skip(found_token))
				{ break; }
			reset_buffer(lex->last_lexeme);
		}
	}

	if (found_token == T_DIRECTIVE) {
		static void* directive_tab[][2] = {
			{ (void*)T_SKIP,		"$SKIP" },
			{ (void*)T_TOKEN,		"$TOKEN" },
			{ (void*)T_KEYWORD,		"$KEYWORD" },
			{ (void*)T_IGCASE,		"$IGCASE" },
			{ (void*)T_EXTERN,		"$EXTERN" },
			{ (void*)T_START,		"$START" },
			{ (void*)T_ALIAS,		"$ALIAS" },
			{ (void*)T_PRODUCTION,	"$PRODUCTION" },
			{ (void*)T_PRECEDENCE,	"$PRECEDENCE" },
			{ (void*)T_EMPTY,		"$EMPTY" },
			{ (void*)T_LEFT,		"$LEFT" },
			{ (void*)T_RIGHT,		"$RIGHT" },
			{ (void*)T_NONASSOC,	"$NONASSOC" },
			{ (void*)T_MIMIC,		"$MIMIC" },
		};
		for (size_t i = 0; i < *(&directive_tab + 1)- directive_tab; ++i) {
			if (!strcmp(C_LEXEME(lex), directive_tab[i][1]))
				{ return ((long)*directive_tab[i]); }
		}
		fprintf(stderr, "Bad directive <%s>.\n", C_LEXEME(lex));
	}
	lex->last_token = -1;
	return (found_token);
}

int
peek_token(lexer_t* lex) {
	if (!lex)
		{ return (-1); }
	if (lex->last_token == -1)
		{ lex->last_token = advance_token(lex); }
	return (lex->last_token);
}

bool
in_first(lexer_t* lex, ...) {
	va_list args;
	va_start(args, lex);
	int token;
	while ((token = va_arg(args, int)) != -1) {
		if (token == peek_token(lex))
			{ return (true); }
	}
	va_end(args);
	return (false);
}

int
is_final_state(int state) {
	for (size_t i = 0; i < SIZE_FINAL_TAB; ++i) {
		if (fgfx_final_table[i][0] == state)
			{ return (fgfx_final_table[i][1]); }
	}
	return (T_ERROR);
}

