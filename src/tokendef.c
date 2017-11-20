#include "tokendef.h"
#include "fgfl.lex.h"

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
strip_token(token_spec_t* spec) {
	reset_buffer(spec->last_lexeme);
	while (1) {
		int token = get_next_token(spec);
		if (token != TSPACE && token != TCOM)
			{ return (token); }
		reset_buffer(spec->last_lexeme);
	}
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
		for (size_t i = 0; i < size_vector(spec->entry_lst); ++i)
			{ del_token_entry(at_vector(spec->entry_lst, i)); }
		del_vector(spec->entry_lst);
		FREE(spec);
	}
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
	puts(entry->name);
	if (!entry->name)
		{ return (-1); }
	entry->phase = AST;
	for (size_t i = 0; i < size_vector(spec->entry_lst); ++i) {
		token_entry_t* current_e = (token_entry_t*)
						at_vector(spec->entry_lst, i);
		if (!strcmp(current_e->name, entry->name))
			{ return (-2); }
	}
	push_back_vector(spec->entry_lst, entry);
	return (0);
}

int
parse_token_entry(token_spec_t* spec) {
	bool empty = true;
	int token;
	while ((token = strip_token(spec)) != TEOF) {
		if (token != TG_IDENT && token != TL_IDENT) {
			fprintf(stderr,
				"Error (%d): Expected ident (Local or not).\n",
				spec->lineno);
			return (-1);
		}
		int err_entry = add_entry_lexeme(spec, token);
		if (err_entry == -1 || err_entry == -2) {
			if (err_entry == -2) {
				fprintf(stderr,
				"Error (%d): Redefining some ident.\n",
				spec->lineno);	
			}
			return (-1);
		}
		if (strip_token(spec) != TEQUAL) {
			fprintf(stderr,
				"Error (%d): No equal sign after an ident.\n",
				spec->lineno);
			return (-1);
		}
		token_entry_t* last_entry = (token_entry_t*)
						back_vector(spec->entry_lst);
		last_entry->reg = regex2ast(spec);
		if (strip_token(spec) != TSEMI) {
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
	(*spec)->entry_lst = vector();
	if (parse_token_entry(*spec))
		{ return (-1); }
	return (0);
}
