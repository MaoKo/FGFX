#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "token_def.h"
#include "regex.h"
#include "nfa.h"
#include "lexer.h"
#include "buffer.h"
#include "error.h"
#include "utils.h"

static void
del_token_entry(token_entry_t* entry) {
	if (entry) {
		if (entry->phase == AST)	
			{ del_node_ast(entry->reg); }
		else if (entry->phase == FRAGMENT)
			{ FREE_FRAG(entry->frag); }
		FREE(entry->name);
		FREE(entry->reg_str);
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

static token_entry_t*
add_entry_lexeme(token_spec_t* spec, int kind) {
	int offset = (kind == LOCAL);
	int index = get_index_vector(spec->entry_lst,
						C_LEXEME(spec->lex) + offset, &cmp_token_entry);

	if (index != -1)
		{ return ((token_entry_t*)AT_VECTOR(spec->entry_lst, index)); }

	token_entry_t* entry = NEW(token_entry_t, 1);
	if (!entry)
		{ return (NULL); }
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
		return (NULL);
	}

	if (entry->kind == KEYWORD)
		{ entry->phase = NONE; }
	else
		{ entry->phase = AST; }

	entry->index = SIZE_VECTOR(spec->entry_lst);
	PUSH_BACK_VECTOR(spec->entry_lst, entry);

	return (entry);
}

static int parse_section(token_spec_t*);
static inline int keyword_section(token_spec_t*);
static inline int token_section(token_spec_t*);
static inline int skip_section(token_spec_t*);
static int keyword_list(token_spec_t*, int);
static int regex_list(token_spec_t*, int);
static int regex_assign(token_spec_t*, token_entry_t*, int);

int
regex_assign(token_spec_t* spec, token_entry_t* entry, int kind_section) {
	if ((!strcmp(entry->name, "ERROR")) || (!strcmp(entry->name, "EOF"))) {
			errorf(CURRENT_LINE(spec->lex), "Can't redefine %s. "
											"Name reserved for special use.\n",
											entry->name);
			return (T_ERROR);
	}

	if (peek_token(spec->lex) == T_EQUAL) {
		if (entry->reg_str|| entry->kind == KEYWORD) {
			errorf(CURRENT_LINE(spec->lex), "Redefinning the token %s.",
											entry->name);
			return (ERROR);
		}
		if (kind_section == T_SKIP)
			{ entry->skip = true; }
		advance_token(spec->lex);
		if (advance_token(spec->lex) != T_REGEX) {
			errorf(CURRENT_LINE(spec->lex), "No found regex after %s = .",
												entry->name);
			return (ERROR);
		}

		unget_c_buffer(LAST_LEXEME(spec->lex), 1);
		size_t save_space = 0;

		char* last_escape = strrchr(C_LEXEME(spec->lex), '\\');
		if ((last_escape && last_escape[-1] != '\\')
				&& is_tab_or_space(last_escape[1]))
			{ save_space = 1; }

		// Get rid of trailing space and tab
		unget_c_buffer(LAST_LEXEME(spec->lex), count_back(C_LEXEME(spec->lex),
			&is_tab_or_space) - save_space);

		size_t front_space = count_front(C_LEXEME(spec->lex) + 1,
				&is_tab_or_space) + 1;

		CURRENT_LINE(spec->lex) += char_in_str(
				C_LEXEME(spec->lex) + front_space, '\n');
		entry->reg_str = strdup(C_LEXEME(spec->lex) + front_space);

		if (!entry->reg_str) {
			errorf(0, "Non enough memory for allocate the regex string.");
			return (ERROR);
		}
	}
	else if (peek_token(spec->lex) == T_ARROW) {
		advance_token(spec->lex);
		if (advance_token(spec->lex) != T_IGCASE) {
			errorf(CURRENT_LINE(spec->lex), "Expected $IGCASE after %s -> .",
													entry->name);
			return (ERROR);
		}
		if (!entry->reg_str) {
			errorf(CURRENT_LINE(spec->lex), "Token %s not defined for $IGCASE.",
													entry->name);
			return (ERROR);
		}
		entry->igcase = true;
	}
	else {
		errorf(CURRENT_LINE(spec->lex), "An equal or an arrow must"
										" follow the token.");
		return (ERROR);
	}
	return (DONE);
}

int
regex_list(token_spec_t* spec, int kind_section) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (in_first(spec->lex, T_GLOBAL_TOK, T_LOCAL_TOK, -1)) {
		token_entry_t* entry = add_entry_lexeme(spec, advance_token(spec->lex));
		if (entry == NULL)
			{ return (ERROR); }
		else if (regex_assign(spec, entry, kind_section) == ERROR)
			{ return (ERROR); }
		if (advance_token(spec->lex) != T_SEMI) {
			errorf(CURRENT_LINE(spec->lex), "Missing a ; after the regex.");
			return (ERROR);
		}
		return (regex_list(spec, kind_section));
	}
	else if (peek_token(spec->lex) != T_RBRACE) {
		errorf(CURRENT_LINE(spec->lex), "Missing a local or  global token in"
							" $%s section", (kind_section == T_TOKEN)
											? "TOKEN" : "SKIP");
		return (ERROR);
	}
	return (DONE);
}

int
keyword_list(token_spec_t* spec, int dummy) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(spec->lex) == T_GLOBAL_TOK) {
		token_entry_t* entry = add_entry_lexeme(spec, KEYWORD);
		++(entry->count);
		if (entry->reg_str) {
			errorf(CURRENT_LINE(spec->lex), "Redefinning token %s in $KEYWORD.",
														entry->name);
			return (ERROR);
		}
		else if (entry->count > 1) {
			warnf(CURRENT_LINE(spec->lex), "%s appear %zu in the"
						" $KEYWORD section.", entry->name, entry->count);
		}
		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);
			return (keyword_list(spec, dummy));
		}
		else if (peek_token(spec->lex) != T_RBRACE) {
			errorf(CURRENT_LINE(spec->lex), "Missing a '}' in"
											" $KEYWORD section");
			return (ERROR);
		}
	}
	else {
		errorf(CURRENT_LINE(spec->lex), "Expected identifier in"
										" the $KEYWORD section.");
		return (ERROR);
	}
	return (DONE);
}

static int
entry_section(token_spec_t* spec, int kind) {
	int (*section_ptr)(token_spec_t*, int) = &regex_list;
	if (kind == T_KEYWORD)
		{ section_ptr = &keyword_list; }

	char next;
	if (advance_token(spec->lex) != kind
			|| (next = '{', advance_token(spec->lex) != T_LBRACE)
			|| (next = '\0', (*section_ptr)(spec, kind) == ERROR)
			|| (next = '}', advance_token(spec->lex) != T_RBRACE)) {
		if (next) {
			errorf(CURRENT_LINE(spec->lex), "Missing a '%c' after"
											" the directive.", next);
		}
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_SEMI) {
		errorf(CURRENT_LINE(spec->lex), "No semicolon after the directive.");
		return (ERROR);
	}
	return (DONE);
}

int
skip_section(token_spec_t* spec) {
	return (entry_section(spec, T_SKIP));
}

int
token_section(token_spec_t* spec) {
	return (entry_section(spec, T_TOKEN));
}

int
keyword_section(token_spec_t* spec) {
	return (entry_section(spec, T_KEYWORD));
}

int
parse_section(token_spec_t* spec) {
	int exit_st = DONE;
	if (peek_token(spec->lex) == T_SKIP)
		{ exit_st = skip_section(spec); }
	else if (peek_token(spec->lex) == T_TOKEN)
		{ exit_st = token_section(spec); }
	else if (peek_token(spec->lex) == T_KEYWORD)
		{ exit_st = keyword_section(spec); }
	else {
		errorf(CURRENT_LINE(spec->lex), "Bad section directive.");
		return (ERROR);
	}
	return (exit_st);
}

int
parse_token_entry(token_spec_t* spec) {
	bool empty = true;
	while (peek_token(spec->lex) != T_EOF) {
		if (parse_section(spec) == ERROR)
			{ return (ERROR); }
		empty = false;
	}
	if (empty)
		{ warnf(CURRENT_LINE(spec->lex), "Empty file."); }
	return (DONE);
}

token_spec_t*
parse_token_def(int filde) {
	token_spec_t* crt_spec = NEW(token_spec_t, 1);
	if (!crt_spec)
		{ return (NULL); }
	memset(crt_spec, 0, sizeof(token_spec_t));
	crt_spec->lex = new_lexer(filde);
	crt_spec->entry_lst = new_vector();
	if (parse_token_entry(crt_spec) == ERROR) {
		del_token_spec(crt_spec);
		return (NULL);
	}
	return (crt_spec);
}
