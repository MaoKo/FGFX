#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "token_def.h"
#include "lexer.h"
#include "utils.h"
#include "regex.h"
#include "nfa.h"
#include "buffer.h"

#define ONLY_TOKEN
#include "fgfx.lex.h"
#undef ONLY_TOKEN

static void
del_token_entry(token_entry_t* entry) {
	if (entry) {
		if (entry->phase == AST)	
			{ del_node_ast(entry->reg); }
		else if (entry->phase == FRAGMENT)
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
add_entry_lexeme(token_spec_t* spec, int kind) {
	token_entry_t* entry = NEW(token_entry_t, 1);
	if (!entry)
		{ return (ERROR); }
	entry->kind = kind;
	switch (entry->kind) {
		case GLOBAL:
		case KEYWORD: entry->used = true;
			break;
		case LOCAL: entry->used = false;
			break;
	}
	int offset = (kind == LOCAL);
	entry->name = strdup(C_LEXEME(spec->lex) + offset);
	entry->igcase = false;
	if (!entry->name) {
		del_token_entry(entry);
		return (ERROR);
	}

	if (entry->kind == KEYWORD)
		{ entry->phase = NONE; }
	else
		{ entry->phase = AST; }
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* current_e = (token_entry_t*)
						AT_VECTOR(spec->entry_lst, i);
		if (!strcmp(current_e->name, entry->name)) {
			del_token_entry(entry);
			return (REDEFINNING);
		}
	}

	PUSH_BACK_VECTOR(spec->entry_lst, entry);
	return (DONE);
}

static int
parse_assignement(token_spec_t* spec) {
	int type = advance_token(spec->lex);
	if ((type != T_GLOBAL_TOK) && (type != T_LOCAL_TOK)) {
		fprintf(stderr, "Error (%d): Expected ident (Local or not).\n",
			CURRENT_LINE(spec->lex));
		return (ERROR);
	}
	int err_entry = add_entry_lexeme(spec,
			((type == T_GLOBAL_TOK) ? GLOBAL : LOCAL));
	if (err_entry == ERROR || err_entry == REDEFINNING) {
		if (err_entry == REDEFINNING) {
			fprintf(stderr,"Error (%d): Redefining some ident.\n",
			CURRENT_LINE(spec->lex));
		}
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_EQUAL) {
		fprintf(stderr, "Error (%d): No equal sign after an ident.\n",
			CURRENT_LINE(spec->lex));
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_REGEX) {
		fprintf(stderr, "Error (%d): No found regex after the equal sign.\n",
			CURRENT_LINE(spec->lex));
		return (ERROR);
	}
	unget_c_buffer(LAST_LEXEME(spec->lex), 1);
	CURRENT_LINE(spec->lex) += char_in_str(C_LEXEME(spec->lex) + 1, '\n');

	token_entry_t* last_entry = (token_entry_t*)BACK_VECTOR(spec->entry_lst);
	last_entry->reg = regex2ast(spec, C_LEXEME(spec->lex) + 1);

	return (DONE);
}

static int
enable_property(token_spec_t* spec, int token) {
	if (token == T_KEYWORD) {
		add_entry_lexeme(spec, KEYWORD);
		return (DONE);
	}
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(spec->entry_lst, i);
		if (!strcmp(entry->name, C_LEXEME(spec->lex))) {
			if (token == T_IGCASE)
				{ entry->igcase = true; }
			else {
				entry->skip = true;
				if (entry->kind == LOCAL) {
					fprintf(stderr,
					"Useless to skip a local symbol: %s.\n",
					entry->name);
				 }
			}
			return (DONE);
		}
	}
	fprintf(stderr, "Identifier %s not found.\n", C_LEXEME(spec->lex));
	return (ERROR);
}

static int
parse_directive(token_spec_t* spec) {
	if (!in_first(spec->lex, T_IGCASE, T_SKIP, T_KEYWORD, -1))
		{ /* ERROR */ return (ERROR); }
	int kind_directive = advance_token(spec->lex);
	if (advance_token(spec->lex) != T_GLOBAL_TOK) {
		fprintf(stderr, "Error (%d): Expected id after %s directive.\n",
			CURRENT_LINE(spec->lex), ((kind_directive == T_IGCASE) ?
						"igcase" : "skip"));
		return (ERROR);
	}
	if (enable_property(spec, kind_directive) == ERROR)
		{ return (ERROR); }		
	while (peek_token(spec->lex) == T_COMMA) {
		advance_token(spec->lex);
		if (advance_token(spec->lex) != T_GLOBAL_TOK) {
			fprintf(stderr, "Error (%d): Expected id after a comma.\n",
				CURRENT_LINE(spec->lex));
			return (ERROR);
		}
		if (enable_property(spec, kind_directive) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

int
parse_token_entry(token_spec_t* spec) {
	bool empty = true;
	int token = T_ERROR;
	while ((token = peek_token(spec->lex)) != T_EOF) {
		if (!in_first(spec->lex, T_IGCASE, T_SKIP, T_KEYWORD, -1))
			{ parse_assignement(spec); }
		else
			{ parse_directive(spec); }
		if (advance_token(spec->lex) != T_SEMI) {
			fprintf(stderr,
				"Error (%d): No semicolon after the regex.\n",
				CURRENT_LINE(spec->lex));
			return (ERROR);
		}
		empty = false;
	}

	if (empty)
		{ fprintf(stderr, "Warning file empty.\n"); }
	return (DONE);
}

int
regex_gen(char const* pathname, token_spec_t** spec) {
	if (!spec)
		{ return (ERROR); }
	token_spec_t* crt_spec = *spec = NEW(token_spec_t, 1);
	int filde = 0;
	if (!crt_spec || ((filde = open(pathname, O_RDONLY)) == -1))
		{ return (ERROR); }
	memset(crt_spec, 0, sizeof(token_spec_t));
	crt_spec->lex = new_lexer(filde);
	crt_spec->entry_lst = new_vector();
	if (parse_token_entry(crt_spec))
		{ return (ERROR); }
	return (DONE);
}
