#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "lexer.h"
#include "fgfx.lex.h"
#include "error.h"
#include "utils.h"
#include "buffer.h"

lexer_t*
new_lexer(int filde) {
	lexer_t* lex = NEW(lexer_t, 1);

	if (!lex)
		{ return (NULL); }
	memset(lex, 0, sizeof(*lex));

	lex->filde = filde;

	lex->last_lexeme = new_buffer();
	lex->push_back = new_buffer();

	lex->last_token = NO_TOKEN;
	lex->lineno = START_LINE;

	return (lex);
}

void
del_lexer(lexer_t* lex) {
	if (lex) {
		del_buffer(lex->push_back);
		del_buffer(lex->last_lexeme);
	}
	FREE(lex);
}

static size_t
is_final_state(int state, uint8_t (*final_table)[2]) {
	size_t i = 0;
	while (*final_table[i]) {
		if (*final_table[i] == state)
			{ return (final_table[i][1]); }
		++i;
	}
	return (T_ERROR);
}

static int
get_next_token(lexer_t* lex) {
	static uint8_t (*state_table)[256] = NULL;
	static uint8_t (*final_table)[2] = NULL;

	static int crt_state = INIT_STATE;

	int state = START_STATE;
	int last_match = T_ERROR;
	int rd = 0;

	if (lex->carry_nl)
		{ ++(lex->lineno); }
	lex->carry_nl = false;

	append_buffer(lex->last_lexeme, lex->push_back);
	reset_buffer(lex->push_back);

	switch (crt_state) {
		case S_GLOBAL:
			state_table = fgfx_GLOBAL_state_table;
			final_table = fgfx_GLOBAL_final_table;
			break;

		case S_SEEN_REGEX:
			state_table = fgfx_SEEN_REGEX_state_table;
			final_table = fgfx_SEEN_REGEX_final_table;
			break;
	}

	for (size_t i = 0; i < SIZE_BUFFER(lex->last_lexeme); ++i) {
		state = state_table[state][(int)CHAR_AT(lex->last_lexeme, i)];
		if (is_final_state(state, final_table) != T_ERROR)
			{ last_match = is_final_state(state, final_table); }
	}

	int first_read = true;
	while (state != DEAD_STATE) {
		int exit_st = read(lex->filde, &rd, 1);
		if (!exit_st) {
			if (first_read)
				{ return (T_EOF); }
			break;
		}
		state = state_table[state][rd];
		if (is_final_state(state, final_table) != T_ERROR)
			{ last_match = is_final_state(state, final_table); }
		write_char_buffer(lex->last_lexeme, rd);
		first_read = false;
		if (rd == '\n') {
			if (state != DEAD_STATE)
				{ ++(lex->lineno); }	
			else
				{ lex->carry_nl = true; }
		}
	}

	if (last_match == T_ERROR) {
		errorf(CURRENT_LINE(lex),
							"Lexical error on input '%s'.", C_LEXEME(lex));
	}
	else {
		unget_c_buffer(lex->last_lexeme, 1);
		write_char_buffer(lex->push_back, rd);
	}

	if (fgfx_begin_table[last_match][crt_state])
		{ crt_state = fgfx_begin_table[last_match][crt_state]; }
	return (last_match);
}

static inline bool
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
		{ return (ERROR); }
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
			{ (void*)T_STATE,		"$STATE" },
			{ (void*)T_BEGIN,		"$BEGIN" },
			{ (void*)T_FRAG,		"$FRAG" },
			{ (void*)T_INITIAL,		"$INITIAL" },
			{ (void*)T_EXTERN,		"$EXTERN" },
			{ (void*)T_ALIAS,		"$ALIAS" },
			{ (void*)T_PRODUCTION,	"$PRODUCTION" },
			{ (void*)T_PRECEDENCE,	"$PRECEDENCE" },
			{ (void*)T_MIMIC,		"$MIMIC" },
			{ (void*)T_EMPTY,		"$EMPTY" },
			{ (void*)T_START,		"$START" },
			{ (void*)T_LEFT,		"$LEFT" },
			{ (void*)T_RIGHT,		"$RIGHT" },
			{ (void*)T_NONASSOC,	"$NONASSOC" },
			{ (void*)T_ALL,			"$ALL" },
			{ (void*)T_NONE,		"$NONE" },
//			{ (void*)T_REJECT,		"$REJECT" },
		};
		for (size_t i = 0; i < *(&directive_tab + 1)- directive_tab; ++i) {
			if (!strcmp(C_LEXEME(lex), directive_tab[i][1]))
				{ return ((long)*directive_tab[i]); }
		}
		errorf(CURRENT_LINE(lex), "Bad directive %s.", C_LEXEME(lex));
		return (T_ERROR);
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

