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

static inline int
cmp_token_entry(token_entry_t* entry, char const* str) {
	return (strcmp(entry->name, str));
}

static int
add_entry_lexeme(token_spec_t* spec, int kind) {
	int offset = (kind == LOCAL);
	int i = get_index_vector(spec->entry_lst,
			C_LEXEME(spec->lex) + offset, &cmp_token_entry);
	if (i != -1)
		{ return (i); }
	token_entry_t* entry = NEW(token_entry_t, 1);
	if (!entry)
		{ return (ERROR); }
	memset(entry, 0, sizeof(token_entry_t));
	entry->kind = kind;
	switch (entry->kind) {
		case GLOBAL:
		case KEYWORD: entry->used = true;
			break;
		case LOCAL: entry->used = false;
			break;
	}
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

	i = SIZE_VECTOR(spec->entry_lst);
	PUSH_BACK_VECTOR(spec->entry_lst, entry);

	return (i);
}

static int
regex_assign(token_spec_t* spec, int index_entry, int kind_section) {
	token_entry_t* entry = (token_entry_t*)
				AT_VECTOR(spec->entry_lst, index_entry);

	if (peek_token(spec->lex) == T_EQUAL) {
		if (entry->reg || entry->kind == KEYWORD) {
			fprintf(stderr, "Error (%d): Redefinning %s.\n",
					CURRENT_LINE(spec->lex), entry->name);
			return (ERROR);
		}
		if (kind_section == T_SKIP)
			{ entry->skip = true; }
		advance_token(spec->lex);
		if (advance_token(spec->lex) != T_REGEX) {
			fprintf(stderr, 
				"Error (%d): No found regex after %s = .\n",
				CURRENT_LINE(spec->lex), entry->name);
			return (ERROR);
		}
		unget_c_buffer(LAST_LEXEME(spec->lex), 1);
		size_t save_space = 0;

		char* last_escape = strrchr(C_LEXEME(spec->lex), '\\');
		if (last_escape && last_escape[-1] != '\\'
				&& is_tab_or_space(last_escape[1]))
			{ save_space = 1; }

		// Get rid of trailing space and tab
		unget_c_buffer(LAST_LEXEME(spec->lex),
			count_back(C_LEXEME(spec->lex),
			&is_tab_or_space) - save_space);

		size_t front_space = count_front(C_LEXEME(spec->lex) + 1,
				&is_tab_or_space) + 1;

		CURRENT_LINE(spec->lex) += char_in_str(
				C_LEXEME(spec->lex) + front_space, '\n');
		entry->reg = regex2ast(spec, C_LEXEME(spec->lex) + front_space);
	}
	else if (peek_token(spec->lex) == T_ARROW) {
		if (!entry->reg) {
			fprintf(stderr, "Token %s not defined.\n", entry->name);
			return (ERROR);
		}
		advance_token(spec->lex);
		if (advance_token(spec->lex) != T_IGCASE) {
			fprintf(stderr, "Expected $IGCASE after %s -> .\n",
				entry->name);
			return (ERROR);
		}
		entry->igcase = true;
	}
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

static int
atom_FGFL(token_spec_t* spec, int kind_symbol) {
	int err_entry = add_entry_lexeme(spec, kind_symbol);
	if (err_entry == ERROR) {
		fprintf(stderr, "Error (%d): Unable to allocate for some entry.\n",
			CURRENT_LINE(spec->lex));
		return (ERROR);
	}
	return (err_entry);
}

static int
regex_list(token_spec_t* spec, int kind_section) {
	if (in_first(spec->lex, T_GLOBAL_TOK, T_LOCAL_TOK, -1)) {
		int index_entry = atom_FGFL(spec,
				(advance_token(spec->lex) ==  T_GLOBAL_TOK)
				? GLOBAL : LOCAL);
		if (index_entry == ERROR)
			{ return (ERROR); }
		if (regex_assign(spec, index_entry, kind_section) == ERROR
				|| advance_token(spec->lex) != T_SEMI) {
			/* ERROR */
			return (ERROR);
		}
		return (regex_list(spec, kind_section));
	}
	else if (peek_token(spec->lex) != T_RBRACE) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

static int
keyword_list(token_spec_t* spec, int dummy) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(spec->lex) == T_GLOBAL_TOK) {
		int index_key = atom_FGFL(spec, KEYWORD);
		if (index_key == ERROR) {
			/* ERROR */
			return (ERROR);
		}
		token_entry_t* entry = AT_VECTOR(spec->entry_lst, index_key);
		if (entry->reg) {
			fprintf(stderr, "Redefinning %s.\n", entry->name);
			return (ERROR);
		}
		else if ((size_t)index_key != LAST_INDEX_VECTOR(spec->entry_lst)) {
			fprintf(stderr, "Warning %s appear twice or more "
					"in the keyword section.\n",
				entry->name);
		}
		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);
			return (keyword_list(spec, dummy));
		}
		else if (peek_token(spec->lex) != T_RBRACE) {
			/* ERROR */
			return (ERROR);
		}
	}
	else {
		fprintf(stderr, "Expected identifier in the KEYWORD section.\n");
		return (ERROR);
	}
	return (DONE);
}

static int
entry_section(token_spec_t* spec, int kind) {
	int (*section_ptr)(token_spec_t*, int) = &regex_list;
	if (kind == T_KEYWORD)
		{ section_ptr = &keyword_list; }
	if (advance_token(spec->lex) != kind
			|| advance_token(spec->lex) != T_LBRACE
			|| (*section_ptr)(spec, kind) == ERROR
			|| advance_token(spec->lex) != T_RBRACE) {
		/* ERROR */
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_SEMI) {
		fprintf(stderr,
			"Error (%d): No semicolon after the regex.\n",
			CURRENT_LINE(spec->lex));
		return (ERROR);
	}
	return (DONE);
}

static inline int
skip_section(token_spec_t* spec) {
	return (entry_section(spec, T_SKIP));
}

static inline int
token_section(token_spec_t* spec) {
	return (entry_section(spec, T_TOKEN));
}

static inline int
keyword_section(token_spec_t* spec) {
	return (entry_section(spec, T_KEYWORD));
}

static int
parse_section(token_spec_t* spec) {
	int exit_status = DONE;
	if (peek_token(spec->lex) == T_SKIP)
		{ exit_status = skip_section(spec); }
	else if (peek_token(spec->lex) == T_TOKEN)
		{ exit_status = token_section(spec); }
	else if (peek_token(spec->lex) == T_KEYWORD)
		{ exit_status = keyword_section(spec); }
	else {
		/* ERORR */
		exit_status = ERROR;
	}
	return (exit_status);
}

int
parse_token_entry(token_spec_t* spec) {
	bool empty = true;
	int token;
	while ((token = peek_token(spec->lex)) != T_EOF) {
		if (parse_section(spec) == ERROR)
			{ return (ERROR); }
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
