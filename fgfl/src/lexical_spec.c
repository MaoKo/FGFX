#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "lexical_spec.h"
#include "preprocess_regex.h"
#include "regex.h"
#include "nfa.h"
#include "lexer.h"
#include "buffer.h"
#include "error.h"
#include "utils.h"

static void
del_spec_entry(spec_entry_t* entry) {
	if (entry) {
		if (entry->kind != T_STATE) {
			if (entry->phase == AST)	
				{ del_regex_node(entry->reg); }
			else if (entry->phase == FRAGMENT)
				{ FREE_FRAG(entry->frag); }
			del_bitset(entry->valid_state);
		}
		FREE(entry->name);
		FREE(entry->reg_str);
	}
	FREE(entry);
}

static lexical_spec_t*
new_lexical_spec(int filde) {
	lexical_spec_t* crt_spec = NEW(lexical_spec_t, 1);

	if (!crt_spec)
		{ return (NULL); }
	memset(crt_spec, 0, sizeof(lexical_spec_t));

	crt_spec->lex = new_lexer(filde);
	crt_spec->entry_vect = new_vector();
	crt_spec->state_vect = new_vector();

	crt_spec->start_state = -1;
	crt_spec->miss_regex = true;

	return (crt_spec);
}

void
del_lexical_spec(lexical_spec_t* spec) {
	if (spec) {
		foreach_vector(spec->entry_vect, &del_spec_entry);
		foreach_vector(spec->state_vect, &del_spec_entry);
	
		del_vector(spec->entry_vect);
		del_vector(spec->state_vect);

		del_lexer(spec->lex);
	}
	FREE(spec);
}

static inline int
cmp_token_entry(spec_entry_t* entry, char const* str) {
	return (strcmp(entry->name, str));
}

static spec_entry_t*
add_entry_lexeme(lexical_spec_t* spec, int kind) {
	if ((kind != T_STATE) && ((!strcmp(C_LEXEME(spec->lex), "ERROR"))
							|| (!strcmp(C_LEXEME(spec->lex), "EOF")))) {
		errorf(CURRENT_LINE(spec->lex),
					"Can't redefine %s. Name reserved for special use.",
					C_LEXEME(spec->lex));
		return (NULL);
	}

	int index = get_index_vector(spec->entry_vect,
							C_LEXEME(spec->lex), &cmp_token_entry);

	spec_entry_t* entry = NULL;
	if (index != -1)
		{ entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, index); }
	else {
		index = get_index_vector(spec->state_vect,
							C_LEXEME(spec->lex), &cmp_token_entry);
		if (index != -1)
			{ entry = (spec_entry_t*)AT_VECTOR(spec->state_vect, index); }
	}

	if (entry) {
		if (entry->kind != kind) {
			errorf(0, "Attempt to define %s into a %s but it is a %s.",
				entry->name, KIND_IN2_STR(kind), KIND_IN2_STR(entry->kind));
			return (NULL);
		}
		return (entry);
	}

	vector_t* src_vect = ((kind == T_STATE)
						? spec->state_vect : spec->entry_vect);

	entry = NEW(spec_entry_t, 1);
	if (!entry)
		{ return (NULL); }
	memset(entry, 0, sizeof(*entry));

	entry->kind = kind;
	entry->name = strdup(C_LEXEME(spec->lex));

	if (!entry->name) {
		del_spec_entry(entry);
		return (NULL);
	}

	if (entry->kind == T_TERMINAL) {
		entry->phase = AST;
		entry->begin_state = -1;
	}

	entry->index = SIZE_VECTOR(src_vect);
	PUSH_BACK_VECTOR(src_vect, entry);

	return (entry);
}

static int spec_state_prefix(lexical_spec_t*, bitset_t**, bool*);
static int spec_regex_assign(lexical_spec_t*, spec_entry_t*, int);

static int spec_regex_property(lexical_spec_t*, spec_entry_t*);
static int spec_change_state(lexical_spec_t*, spec_entry_t*);

static int spec_state_token_list(lexical_spec_t*, int);
static int spec_regex_list(lexical_spec_t*, int);

int
spec_regex_property(lexical_spec_t* spec, spec_entry_t* entry) {
	int kind_property = peek_token(spec->lex);

	if ((kind_property == T_IGCASE) || (kind_property == T_FRAG)) {
		advance_token(spec->lex);
		((kind_property == T_IGCASE)
				? (entry->is_igcase = true) : (entry->is_frag = true));

		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);

			if (peek_token(spec->lex) == T_RBRACE)
				{ return (DONE); }
			else if ((kind_property == T_IGCASE)
						&& (peek_token(spec->lex) != T_FRAG)) {
				errorf(CURRENT_LINE(spec->lex), "$FRAG must follow $IGCASE.");
				return (ERROR);
			}
			else if ((kind_property == T_FRAG)
						&& (peek_token(spec->lex) != T_IGCASE)) {
				errorf(CURRENT_LINE(spec->lex), "$IGCASE must follow $FRAG.");
				return (ERROR);
			}

			advance_token(spec->lex);
			((kind_property == T_IGCASE)
				? (entry->is_frag = true) : (entry->is_igcase = true));
		}
	}
	else {
		errorf(CURRENT_LINE(spec->lex), "A $FRAG or $IGCASE must appear.");
		return (ERROR);
	}
	return (DONE);
}

int
spec_change_state(lexical_spec_t* spec, spec_entry_t* entry) {
	if (advance_token(spec->lex) != T_LPAREN) {
		/* ERROR */
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_BEGIN) {
		/* ERROR */
		return (ERROR);
	}
	if (advance_token(spec->lex) != T_TERMINAL) {
		/* ERROR */
		return (ERROR);
	}

	spec_entry_t* crt_state = add_entry_lexeme(spec, T_STATE);
	if (!crt_state)
		{ return (ERROR); }
	entry->begin_state = GET_INDEX(crt_state);	
	if (!entry->is_frag)
		{ crt_state->is_reach = true; }

	if (advance_token(spec->lex) != T_RPAREN) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);	
}

int
spec_state_prefix(lexical_spec_t* spec,
								bitset_t** valid_state, bool* all_state) {
	if (peek_token(spec->lex) == T_LPAREN) {
		advance_token(spec->lex);
		bitset_t* state_seen = new_bitset();

		if (peek_token(spec->lex) == T_STAR) {
			(*all_state) = true;
			advance_token(spec->lex);
		}
		else if (peek_token(spec->lex) == T_TERMINAL) {
			while (peek_token(spec->lex) == T_TERMINAL) {
				advance_token(spec->lex);

				spec_entry_t* crt_state = add_entry_lexeme(spec, T_STATE);
				if (!crt_state) {
					del_bitset(state_seen);
					return (ERROR);
				}
				crt_state->is_used = true;

				if (IS_PRESENT(state_seen, GET_INDEX(crt_state))) {
					warnf(CURRENT_LINE(spec->lex),
								"State %s already present in the state list.",
								crt_state->name);
				}
				else 
					{ ADD_BITSET(state_seen, GET_INDEX(crt_state)); }

				if (peek_token(spec->lex) == T_COMMA)
					{ advance_token(spec->lex); }
			}
		}
		else {
			errorf(CURRENT_LINE(spec->lex),
				"Only a state list or the '*' must follow the open paren.");
			return (ERROR);
		}

		if (advance_token(spec->lex) != T_RPAREN) {
			errorf(CURRENT_LINE(spec->lex),
				"Missing a close paren after the state list.");
			del_bitset(state_seen);
			return (ERROR);
		}
		(*valid_state) = state_seen;
	}
	return (DONE);
}

int
spec_regex_assign(lexical_spec_t* spec,
							spec_entry_t* entry, int kind_section) {
	if (peek_token(spec->lex) == T_EQUAL) {
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
		char* reg_str = strdup(C_LEXEME(spec->lex) + front_space);

		if (!reg_str) {
			errorf(0, "Non enough memory for allocate the regex string.");
			return (ERROR);
		}
		else if (entry->reg_str) {
			errorf(CURRENT_LINE(spec->lex),
					"Attempt to re-affect '%s' into the token %s who "
								"has already a regex '%s'.", reg_str,
								entry->name, entry->reg_str);
			FREE(reg_str);
			return (ERROR);
		}
	
		entry->reg_str = reg_str;
		if (peek_token(spec->lex) == T_ARROW) {
			advance_token(spec->lex);
			if (advance_token(spec->lex) != T_LBRACE) {
				errorf(CURRENT_LINE(spec->lex),
							"An open brace must follow the '->'.");
				return (ERROR);
			}
			else if (spec_regex_property(spec, entry) == ERROR)
				{ return (ERROR); }
			else if (advance_token(spec->lex) != T_RBRACE) {
				errorf(CURRENT_LINE(spec->lex),
							"A close brace must follow the '->'.");
				return (ERROR);
			}

		}
		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);
			if (spec_change_state(spec, entry) == ERROR)
				{ return (ERROR); }
		}
	}
	else {
		errorf(CURRENT_LINE(spec->lex),
				"An equal sign ('=') must follow the token %s.", entry->name);
		return (ERROR);
	}
	return (DONE);
}

int
spec_regex_list(lexical_spec_t* spec, int kind_section) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }

	bool all_state = false;
	bitset_t* state_seen = NULL;

	if (spec_state_prefix(spec, &state_seen, &all_state) == ERROR)
		{ return (ERROR); }

	if (peek_token(spec->lex) == T_TERMINAL) {
		spec->miss_regex = false;
		spec_entry_t* entry = add_entry_lexeme(spec, advance_token(spec->lex));

		if (!entry)
			{ return (ERROR); }

		entry->all_state = all_state;
		entry->valid_state = state_seen;

		if (spec_regex_assign(spec, entry, kind_section) == ERROR)
			{ return (ERROR); }
		else if (advance_token(spec->lex) != T_SEMI) {
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
spec_state_token_list(lexical_spec_t* spec, int kind_section) {
	if (peek_token(spec->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(spec->lex) == T_TERMINAL) {
		spec_entry_t* entry = add_entry_lexeme(spec, kind_section);
		if (!entry)
			{ return (ERROR); }

		++(entry->count);
		if (kind_section == T_STATE)
			{ entry->is_defined = true; }

		if (kind_section == T_STATE && peek_token(spec->lex) == T_BARROW) {
			advance_token(spec->lex);
			if (advance_token(spec->lex) != T_INITIAL) {
				errorf(CURRENT_LINE(spec->lex),
							"Missing $INITIAL after the '=>' in the $T_STATE.");
				return (ERROR);
			}
			if (spec->start_state != -1)
				{ warnf(CURRENT_LINE(spec->lex), "Override initial state."); }
			spec->start_state = GET_INDEX(entry);
		}

		if (peek_token(spec->lex) == T_COMMA) {
			advance_token(spec->lex);
			return (spec_state_token_list(spec, kind_section));
		}
	}
	else {
		errorf(CURRENT_LINE(spec->lex),
							"Expected identifier in the %s section.",
							STATE_TOKEN_LST_SECT(kind_section));
		return (ERROR);
	}
	return (DONE);
}

static int
spec_token_entry_section(lexical_spec_t* spec) {
	int kind = advance_token(spec->lex);
	int (*section_ptr)(lexical_spec_t*, int) = NULL;

	switch (kind) {
		case T_TOKEN: case T_SKIP:
			section_ptr = &spec_regex_list;
			break;
		case T_STATE: case T_KEYWORD:
			section_ptr = &spec_state_token_list;
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
spec_token_section(lexical_spec_t* spec) {
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

lexical_spec_t*
parse_lexical_spec(int filde) {
	lexical_spec_t* crt_spec = new_lexical_spec(filde);
	if (spec_token_section(crt_spec) == ERROR) {
		del_lexical_spec(crt_spec);
		return (NULL);
	}
	return (crt_spec);
}

static int
check_validity_token(lexical_spec_t* spec) {
	bool state_present = !EMPTY_VECTOR(spec->state_vect);
	int exit_st = DONE;

	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* crt_entry = (spec_entry_t*)
										AT_VECTOR(spec->entry_vect, i);

		if (crt_entry->kind == T_TERMINAL) {
			if (crt_entry->is_frag) {
				if (!crt_entry->is_used) {
					warnf(0, "Local token %s is defined but not used.",
										crt_entry->name);
				}
				if (crt_entry->begin_state != -1) {
					warnf(0, "Useless to do a $BEGIN on local token %s.",
										crt_entry->name);
				}
			}
			if (state_present) {
				if (!crt_entry->valid_state) {
					if (!crt_entry->all_state) {
						errorf(0, "Token %s is not prefixed by any state.",
											crt_entry->name);
						exit_st = ERROR;
					}
				}
				else if (crt_entry->begin_state != -1
							&& IS_PRESENT(crt_entry->valid_state,
								(size_t)crt_entry->begin_state)) {
					warnf(0, "Useless to do a $BEGIN on state %s when"
									" this state is already active.",
									((spec_entry_t*)AT_VECTOR(spec->state_vect,
									(size_t)crt_entry->begin_state))->name);
				}
			}
		}
		else if ((crt_entry->kind == T_KEYWORD) && (crt_entry->count > 1)) {
			warnf(0, "The token '%s' appear %zu in the $KEYWORD section.",
					crt_entry->name, crt_entry->count);
		}
	}
	return (exit_st);
}

static void
del_useless_state(lexical_spec_t* spec, size_t base) {
	erase_vector(spec->state_vect, base);
}

static int
check_validity_state(lexical_spec_t* spec) {
	if (spec->start_state == -1) {
		if (EMPTY_VECTOR(spec->state_vect))
			{ return (DONE); }
		warnf(0, "Initial state not defined. Default set to state 0.");
		spec->start_state = 0;
	}

	int exit_st = DONE;
	for (size_t i = 0; i < SIZE_VECTOR(spec->state_vect); ++i) {
		spec_entry_t* crt_state = (spec_entry_t*)AT_VECTOR(spec->state_vect, i);
		if (!crt_state->is_defined) {
			errorf(0, "The state %s is used but not defined.",
								crt_state->name);
			exit_st = ERROR;
		}
		if (!crt_state->is_used) {
			warnf(0, "The state %s is defined but not used.",
								crt_state->name);
			del_useless_state(spec, GET_INDEX(crt_state));
		}
		if (!crt_state->is_reach
				&& ((size_t)spec->start_state != GET_INDEX(crt_state))) {
			warnf(0, "The state %s is actualy unreachable.",
								crt_state->name);
			del_useless_state(spec, GET_INDEX(crt_state));
		}
		if (crt_state->count > 1) {
			warnf(0, "The state '%s' appear %zu in the $STATE section.",
								crt_state->name, crt_state->count);
		}
	}
	if ((exit_st != ERROR) && (SIZE_VECTOR(spec->state_vect) == 1)) {
		warnf(0, "Useless to have only 1 state. It's an implicit state.");
		POP_BACK_VECTOR(spec->state_vect);
		spec->start_state = -1;
	}
	return (exit_st);
}

int
spec_sanity_check(lexical_spec_t* spec) {
	if (!spec)
		{ return (ERROR); }
	else if (build_regex(spec) == ERROR)
		{ return (ERROR); }
	else if (check_validity_token(spec) == ERROR)
		{ return (ERROR); }
	else if (check_validity_state(spec) == ERROR)
		{ return (ERROR); }
	return (DONE);
}

#ifdef PRINT_DEBUG

void
print_token_entry(lexical_spec_t* spec) {
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* crt_entry = AT_VECTOR(spec->entry_vect, i);
		printf("Token %s, is_igcase %d, is_frag %d.\n",
					crt_entry->name, crt_entry->is_igcase, crt_entry->is_frag);
		if (crt_entry->valid_state)
			{ print_bitset(crt_entry->valid_state); }
	}
}

#endif /* PRINT_DEBUG */
