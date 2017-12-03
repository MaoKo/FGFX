#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "tokendef.h"
#include "lexer.h"
#include "utils.h"
#include "regex.h"
#include "nfa.h"
#include "buffer.h"

#define ONLY_TOKEN
#include "fgfl.lex.h"
#undef ONLY_TOKEN

static void
del_token_entry(token_entry_t* entry) {
	if (entry) {
		if (entry->phase == AST)	
			{ del_node_ast(entry->reg); }
		else
			{ FREE_FRAG(entry->frag); }
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
		del_lexer(spec->lex);
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
	entry->name = strdup(BODY_BUFFER(LAST_LEXEME(spec->lex)) + offset);
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
			CURRENT_LINE(spec->lex));
		return (-1);
	}
	int err_entry = add_entry_lexeme(spec, type);
	if (err_entry == -1 || err_entry == -2) {
		if (err_entry == -2) {
			fprintf(stderr,"Error (%d): Redefining some ident.\n",
			CURRENT_LINE(spec->lex));
		}
		return (-1);
	}
	if (advance_token(spec->lex) != TEQUAL) {
		fprintf(stderr, "Error (%d): No equal sign after an ident.\n",
			CURRENT_LINE(spec->lex));
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
		if (!strcmp(entry->name, BODY_BUFFER(LAST_LEXEME(spec->lex)))) {
			entry->igcase = true;
			return (0);
		}
	}
	fprintf(stderr, "Identifier %s not found.\n",
		BODY_BUFFER(LAST_LEXEME(spec->lex)));
	return (-1);
}

static int
parse_directive(token_spec_t* spec) {
	if (advance_token(spec->lex) != TG_IDENT) {
		fprintf(stderr, "Error (%d): Expected id after igcase directive.\n",
			CURRENT_LINE(spec->lex));
		return (-1);
	}
	if (enable_igcase(spec) == -1)
		{ return (-1); }
	while (peek_token(spec->lex) == TCOMMA) {
		if (advance_token(spec->lex) != TG_IDENT) {
			fprintf(stderr, "Error (%d): Expected id after the comma.\n",
				CURRENT_LINE(spec->lex));
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
	while ((token = advance_token(spec->lex)) != TEOF) {
		if (token != TIGCASE)
			{ parse_assignement(spec, token); }
		else
			{ parse_directive(spec); }
		if (advance_token(spec->lex) != TSEMI) {
			fprintf(stderr,
				"Error (%d): No semicolon after the regex.\n",
				CURRENT_LINE(spec->lex));
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
	token_spec_t* crt_spec = *spec = NEW(token_spec_t, 1);
	int filde = 0;
	if (!crt_spec || ((filde = open(pathname, O_RDONLY)) == -1))
		{ return (-1); }
	memset(crt_spec, 0, sizeof(token_spec_t));
	crt_spec->lex = new_lexer(filde);
	crt_spec->entry_lst = new_vector();
	if (parse_token_entry(crt_spec))
		{ return (-1); }
	return (0);
}
