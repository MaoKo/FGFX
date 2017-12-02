#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "fgfl.lex.h"
#include "tokendef.h"
#include "utils.h"
#include "regex.h"
#include "nfa.h"
#include "buffer.h"

int
is_final_state(int state) {
	for (size_t i = 0; i < SIZE_FINAL_TAB; ++i) {
		if (final_table[i][0] == state)
			{ return (final_table[i][1]); }
	}
	return (TNONE);
}

static int
get_next_token(token_spec_t* spec) {
	int rd, state = START_STATE, last_match = TNONE;
	int c = 0;

	if (!spec->last_lexeme)
		{ spec->last_lexeme = buffer(); }
	if (spec->last_char != -1) {
		state = state_table[state][spec->last_char];
		last_match = is_final_state(state);
		write_char_buffer(spec->last_lexeme, spec->last_char);
		if (spec->last_char == '\n')
			{ ++spec->lineno; }
	}

	int first_read = true;
	while (state != DEAD_STATE) {
		rd = read(spec->filde, &c, 1);
		if (!rd) {
			if (first_read)
				{ return (TEOF); }
			break;
		}
		state = state_table[state][c];
		if (is_final_state(state))
			{ last_match = is_final_state(state); }
		write_char_buffer(spec->last_lexeme, c);
		first_read = false;
		if (state != DEAD_STATE && c == '\n')
			{ ++spec->lineno; }
	}
	
	if (last_match == TNONE) {
		fprintf(stderr, "Lexical Error <%d:%s>.\n",
			spec->lineno, body_buffer(spec->last_lexeme));
		spec->last_char = -1;
	}
	else {
		spec->last_char = c;
		unget_c_buffer(spec->last_lexeme, 1);
	}
	return (last_match);
}

static int
advance_token(token_spec_t* spec) {
	reset_buffer(spec->last_lexeme);
	while (1) {
		spec->last_token = get_next_token(spec);
		if (spec->last_token != TSPACE && spec->last_token != TCOM)
			{ return (spec->last_token); }
		reset_buffer(spec->last_lexeme);
	}
}

static int
peek_token(token_spec_t* spec) {
	if (spec->last_token == -1)
		{ spec->last_token = advance_token(spec); }
	return (spec->last_token);
}

static void
del_token_entry(token_entry_t* entry) {
	if (entry) {
		if (entry->phase == AST)	
			{ del_node_ast(entry->reg); }
		else
			{ /* TODO: Del NFA Fragment */ }
		FREE(entry->name);
		FREE(entry);
	}
}

void
del_token_spec(token_spec_t* spec) {
	if (spec) {
		for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i)
			{ del_token_entry(AT_VECTOR(spec->entry_lst, i)); }
		del_vector(spec->entry_lst);
	}
	FREE(spec);
}

int
add_entry_lexeme(token_spec_t* spec, int token) {
	token_entry_t* entry = NEW(token_entry_t, 1);
	if (!entry)
		{ return (-1); }
	entry->used = true;
	if (token == TL_IDENT) {
		entry->local = true;
		entry->used = false;
	}
	int offset = (token == TL_IDENT);
	entry->name = strdup(body_buffer(spec->last_lexeme) + offset);
	entry->igcase = false;
	if (!entry->name)
		{ return (-1); }
	entry->phase = AST;
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* current_e = (token_entry_t*)
						AT_VECTOR(spec->entry_lst, i);
		if (!strcmp(current_e->name, entry->name))
			{ return (-2); }
	}
	PUSH_BACK_VECTOR(spec->entry_lst, entry);
	return (0);
}

static int
parse_assignement(token_spec_t* spec, int type) {
	if (type != TG_IDENT && type != TL_IDENT) {
		fprintf(stderr, "Error (%d): Expected ident (Local or not).\n",
			spec->lineno);
		return (-1);
	}
	int err_entry = add_entry_lexeme(spec, type);
	if (err_entry == -1 || err_entry == -2) {
		if (err_entry == -2) {
			fprintf(stderr,"Error (%d): Redefining some ident.\n",
			spec->lineno);	
		}
		return (-1);
	}
	if (advance_token(spec) != TEQUAL) {
		fprintf(stderr, "Error (%d): No equal sign after an ident.\n",
			spec->lineno);
		return (-1);
	}
	token_entry_t* last_entry = (token_entry_t*)BACK_VECTOR(spec->entry_lst);
	last_entry->reg = regex2ast(spec);
	return (0);
}

static int
enable_igcase(token_spec_t* spec) {
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(spec->entry_lst, i);
		if (!strcmp(entry->name, body_buffer(spec->last_lexeme))) {
			entry->igcase = true;
			return (0);
		}
	}
	fprintf(stderr, "Identifier %s not found.\n",
		body_buffer(spec->last_lexeme));
	return (-1);
}

static int
parse_directive(token_spec_t* spec) {
	if (advance_token(spec) != TG_IDENT) {
		fprintf(stderr, "Error (%d): Expected id after igcase directive.\n",
			spec->lineno);
		return (-1);
	}
	if (enable_igcase(spec) == -1)
		{ return (-1); }
	while (peek_token(spec) == TCOMMA) {
		if (advance_token(spec) != TG_IDENT) {
			fprintf(stderr,
				"Error (%d): Expected id after the comma.\n",
				spec->lineno);
			return (-1);
		}
		if (enable_igcase(spec) == -1)
			{ return (-1); }
	}
	return (0);
}

int
parse_token_entry(token_spec_t* spec) {
	bool empty = true;
	int token;
	while ((token = advance_token(spec)) != TEOF) {
		if (token != TIGCASE)
			{ parse_assignement(spec, token); }
		else
			{ parse_directive(spec); }
		if (advance_token(spec) != TSEMI) {
			fprintf(stderr,
				"Error (%d): No semicolon after the regex.\n",
				spec->lineno);
			return (-1);
		}
		empty = false;
	}

	if (empty)
		{ /* FILE EMPTY */ }
	return (0);
}

int
Reggen(char const* pathname, token_spec_t** spec) {
	if (!spec)
		{ return (-1); }
	*spec = NEW(token_spec_t, 1);
	if (!*spec || (((*spec)->filde = open(pathname, O_RDONLY)) == -1))
		{ return (-1); }
	(*spec)->last_char = -1; (*spec)->last_lexeme = NULL;
	(*spec)->master = 0; (*spec)->lineno = 1;
	(*spec)->entry_lst = new_vector(); (*spec)->last_token = -1;
	if (parse_token_entry(*spec))
		{ return (-1); }
	return (0);
}
