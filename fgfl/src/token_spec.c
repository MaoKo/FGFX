#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "token_spec.h"
#include "preprocess_regex.h"
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
	}
	FREE(entry);
}

static token_spec_t*
new_token_spec(int filde) {
	token_spec_t* crt_spec = NEW(token_spec_t, 1);

	if (!crt_spec)
		{ return (NULL); }
	memset(crt_spec, 0, sizeof(token_spec_t));

	crt_spec->lex = new_lexer(filde);
	crt_spec->entry_lst = new_vector();
	crt_spec->start_state = -1;

	return (crt_spec);
}

void
del_token_spec(token_spec_t* spec) {
	if (spec) {
		for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i)
			{ del_token_entry((token_entry_t*)AT_VECTOR(spec->entry_lst, i)); }
		del_vector(spec->entry_lst);
		del_vector(spec->state);
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

	int index = get_index_vector(spec->entry_lst, C_LEXEME(spec->lex) + offset,
										&cmp_token_entry);
	if (index != -1)
		{ return ((token_entry_t*)AT_VECTOR(spec->entry_lst, index)); }

	index = get_index_vector(spec->state, C_LEXEME(spec->lex) + offset,
										&cmp_token_entry);
	if (index != -1)
		{ return ((token_entry_t*)AT_VECTOR(spec->state, index)); }

	vector_t* dest = ((kind == T_STATE) ? spec->state : spec->entry_lst);
	token_entry_t* entry = NEW(token_entry_t, 1);

	if (!entry)
		{ return (NULL); }
	memset(entry, 0, sizeof(*entry));

	entry->kind = kind;
	switch (entry->kind) {
		case GLOBAL:
		case KEYWORD: entry->is_used = true;
			break;
		case LOCAL: entry->is_used = false;
			break;
	}

	entry->name = strdup(C_LEXEME(spec->lex) + offset);
	if (!entry->name) {
		del_token_entry(entry);
		return (NULL);
	}

	if (entry->kind == GLOBAL || entry->kind == LOCAL) {
		entry->igcase = false;
		entry->phase = AST;
	}

	entry->index = SIZE_VECTOR(spec->entry_lst);
	PUSH_BACK_VECTOR(dest, entry);

	return (entry);
}

static int spec_token_list(token_spec_t*, int);
static int spec_regex_list(token_spec_t*, int);
static int spec_regex_assign(token_spec_t*, token_entry_t*, int);

int
spec_regex_assign(token_spec_t* spec,
							token_entry_t* entry, int kind_section) {

	if ((!strcmp(entry->name, "ERROR")) || (!strcmp(entry->name, "EOF"))) {
			errorf(CURRENT_LINE(spec->lex),
					"Can't redefine %s. Name reserved for special use.",
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
			errorf(CURRENT_LINE(spec->lex),
						"No found regex after %s = .", entry->name);
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
			errorf(CURRENT_LINE(spec->lex),
							"Expected $IGCASE after %s -> .", entry->name);
			return (ERROR);
		}
		if (!entry->reg_str) {
			errorf(CURRENT_LINE(spec->lex),
							"Token %s not defined for $IGCASE.", entry->name);
			return (ERROR);
		}
		entry->igcase = true;
	}
	else {
		errorf(CURRENT_LINE(spec->lex),
						"An equal or an arrow must follow the token.");
		return (ERROR);
	}
	return (DONE);
}

int
spec_regex_list(token_spec_t* spec, int kind_section) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (in_first(spec->lex, T_GLOBAL_TOK, T_LOCAL_TOK, -1)) {
		spec->miss_regex = true;
		token_entry_t* entry = add_entry_lexeme(spec, advance_token(spec->lex));

		if (entry == NULL)
			{ return (ERROR); }
		else if (spec_regex_assign(spec, entry, kind_section) == ERROR)
			{ return (ERROR); }
		if (advance_token(spec->lex) != T_SEMI) {
			errorf(CURRENT_LINE(spec->lex), "Missing a ';' after the regex.");
			return (ERROR);
		}
		return (spec_regex_list(spec, kind_section));
	}
	else if (peek_token(spec->lex) != T_RBRACE) {
		errorf(CURRENT_LINE(spec->lex),
						"Missing a local or global token in %s section",
							REGEX_LST_SECT(kind_section));
		return (ERROR);
	}
	return (DONE);
}

int
spec_token_list(token_spec_t* spec, int kind_section) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(spec->lex) == T_GLOBAL_TOK) {
		if (kind_section == T_STATE && !spec->state)
			{ spec->state = new_vector(); }

		token_entry_t* entry = add_entry_lexeme(spec, kind_section);
		++(entry->count);

		if (entry->reg_str) {
			errorf(CURRENT_LINE(spec->lex),
								"Redefinning token %s in %s.",
								entry->name, TOKEN_LST_SECT(kind_section));
			return (ERROR);
		}
		else if (entry->kind != kind_section) {
			errorf(CURRENT_LINE(spec->lex),
					"Redefinning token %s from section %s to %s.", entry->name, 
					TOKEN_LST_SECT(entry->kind), TOKEN_LST_SECT(kind_section));
		}

		if (kind_section == T_STATE && peek_token(spec->lex) == T_BARROW) {
			advance_token(spec->lex);
			if (advance_token(spec->lex) != T_INITIAL) {
				errorf(CURRENT_LINE(spec->lex),
							"Missing $INITIAL after the '=>' in the $STATE.");
				return (ERROR);
			}
			if (spec->start_state != -1)
				{ warnf(CURRENT_LINE(spec->lex), "Override initial state."); }
			spec->start_state = GET_INDEX(entry);
		}

		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);
			return (spec_token_list(spec, kind_section));
		}
	}
	else {
		errorf(CURRENT_LINE(spec->lex),
						"Expected identifier in the %s section.",
							TOKEN_LST_SECT(kind_section));
		return (ERROR);
	}
	return (DONE);
}

static int
spec_token_entry_section(token_spec_t* spec) {
	int kind = advance_token(spec->lex);
	int (*section_ptr)(token_spec_t*, int) = NULL;

	switch (kind) {
		case T_TOKEN: case T_SKIP:
			section_ptr = &spec_regex_list;
			break;
		case T_STATE: case T_KEYWORD:
			section_ptr = &spec_token_list;
			break;
	}

	if (!section_ptr) {
		errorf(CURRENT_LINE(spec->lex), "Bad section directive.");
		return (ERROR);
	}

	char miss_c;
	if ((miss_c = '{', advance_token(spec->lex) != T_LBRACE)
			|| (miss_c = '\0', (*section_ptr)(spec, kind) == ERROR)
			|| (miss_c = '}', advance_token(spec->lex) != T_RBRACE)
			|| (miss_c = ';', advance_token(spec->lex) != T_SEMI)) {
		if (miss_c) {
			errorf(CURRENT_LINE(spec->lex),
							"Missing a '%c' after the directive.", miss_c);
		}
		return (ERROR);
	}
	return (DONE);
}

int
spec_token_section(token_spec_t* spec) {
	bool empty = true;
	while (peek_token(spec->lex) != T_EOF) {
		if (spec_token_entry_section(spec) == ERROR)
			{ return (ERROR); }
		empty = false;
	}
	if (empty)
		{ warnf(CURRENT_LINE(spec->lex), "Empty file."); }
	return (DONE);
}

token_spec_t*
parse_token_spec(int filde) {
	token_spec_t* crt_spec = new_token_spec(filde);
	if (spec_token_section(crt_spec) == ERROR) {
		del_token_spec(crt_spec);
		return (NULL);
	}
	return (crt_spec);
}

int
spec_sanity_check(token_spec_t* spec) {
	if (!spec)
		{ return (ERROR); }
	else if (compute_regex(spec) == ERROR)
		{ return (ERROR); }

	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* crt_entry = AT_VECTOR(spec->entry_lst, i);
		if ((crt_entry->kind == LOCAL) && (!crt_entry->is_used))
			{ warnf(0, "Local token %s is not used.", crt_entry->name); }
		else if ((crt_entry->kind == KEYWORD) && (crt_entry->count > 1)) {
			warnf(0, "The token '%s' appear %zu in the $KEYWORD section.",
					crt_entry->name, crt_entry->count);
		}
	}

	for (size_t i = 0; i < SIZE_VECTOR(spec->state); ++i) {
		token_entry_t* crt_entry = AT_VECTOR(spec->state, i);
		if (crt_entry->count > 1) {
			warnf(0, "The state '%s' appear %zu in the $STATE section.",
					crt_entry->name, crt_entry->count);
		}
	}

	return (DONE);
}

#ifdef PRINT_DEBUG

void
print_token_entry(token_spec_t* spec) {
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		// ...	
	}
}

#endif /* PRINT_DEBUG */
