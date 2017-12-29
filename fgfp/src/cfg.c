#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "cfg_production.h"
#include "lexer.h"
#include "utils.h"

cfg_t*
new_cfg(int filde) {
	cfg_t* cfg = NEW(cfg_t, 1);
	if (!cfg)
		{ return (NULL); }
	memset(cfg, 0, sizeof(cfg_t));
	cfg->productions = new_vector();
	cfg->non_terminal = new_vector();
	cfg->terminal = new_vector();
	cfg->token_file = new_vector();
	cfg->lex = new_lexer(filde);
	return (cfg);
}

static void
free_symbol(symbol_t* sym) {
	if (sym) {
		FREE(sym->name);
		if (sym->kind == NON_TERMINAL) {
			del_bitset(sym->first);
			del_bitset(sym->follow);
			del_bitset(sym->prod_lst);
		}
	}
	FREE(sym);
}

void
del_cfg(cfg_t* cfg) {
	if (cfg) {
		foreach_vector(cfg->non_terminal, &free_symbol);	
		del_vector(cfg->non_terminal);
		
		foreach_vector(cfg->terminal, &free_symbol);
		del_vector(cfg->terminal);
	
		foreach_vector(cfg->productions, &del_production);
		del_vector(cfg->productions);
	
		foreach_vector(cfg->token_file, &free);
		del_vector(cfg->token_file);

		del_lexer(cfg->lex);		
	}
	FREE(cfg);
}

static int
cmp_symbol_name(symbol_t const* sym, char const* name) {
	return (strcmp(sym->name, name));
}

static symbol_t*
add_symbol_cfg(cfg_t* cfg, int kind, char const* crt_lexeme) {
	vector_t* dest = (kind == NON_TERMINAL)
						? cfg->non_terminal : cfg->terminal;
	int index = get_index_vector(dest, crt_lexeme, &cmp_symbol_name);
	if (index != -1)
		{ return ((symbol_t*)AT_VECTOR(dest, index)); }
	symbol_t* symbol = NEW(symbol_t, 1);
	if (!symbol)
		{ return (NULL); }
	memset(symbol, 0, sizeof(symbol_t));

	symbol->kind = kind;
	symbol->name = strdup(crt_lexeme);

	int fail = 0;
	if (!symbol->name)
		{ fail = 1; }

	if (kind == LITERAL)
		{ symbol->terminal_alias = -1; }
	else if (kind == TERMINAL) {
		symbol->is_defined = true;
		symbol->precedence = NOT_PREC;
	}
	else {
		symbol->prod_lst = new_bitset();
		if (!symbol->prod_lst)
			{ fail = 1; }
	}

	if (fail) {
		free_symbol(symbol);
		return (NULL);
	}
/*
	if (kind == NON_TERMINAL)
		{ *strchr(sym->name, '>') = EOS; }

	//vector_t** dst = (kind == NON_TERMINAL)
	//? cfg->non_terminal : cfg->terminal;
	//size_t hs = hash_str(sym->name) % HASH_SIZE;
	//else
	//	{ FREE(sym); }
*/
	symbol->index = SIZE_VECTOR(dest);
	PUSH_BACK_VECTOR(dest, symbol);
	return (symbol);
}

static int cfg_section(cfg_t*);

cfg_t*
parse_cfg(int filde) {
	cfg_t* cfg = new_cfg(filde);
	if (cfg_section(cfg) == ERROR) {
		/* ERROR */
		del_cfg(cfg);
		return (NULL);
	}
	return (cfg);
}

void
augment_grammar(cfg_t* cfg) {
	if (!cfg)
		{ return; }
	symbol_t* start = add_symbol_cfg(cfg, NON_TERMINAL, "%start%");
	start->is_defined = true;
	production_t* prod = new_production(start,
					SIZE_VECTOR(cfg->productions));
	add_symbol_rhs(prod, AT_VECTOR(cfg->non_terminal, cfg->goal));
	symbol_t* eof_symbol = add_symbol_cfg(cfg, TERMINAL, "EOF");
	eof_symbol->is_eof = true;
	add_symbol_rhs(prod, eof_symbol);
	ADD_BITSET(start->prod_lst, SIZE_VECTOR(cfg->productions));
	PUSH_BACK_VECTOR(cfg->productions, prod);
	cfg->goal = start->index;
}

// Different kind of section and their associate list.
static int cfg_path_list(cfg_t*);
static int cfg_alias_list(cfg_t*);
static int cfg_production_list(cfg_t*);

static int cfg_production(cfg_t*);
static int cfg_rhs(cfg_t*, symbol_t* lhs);
static int cfg_opt_list(cfg_t*, production_t*);
static int cfg_list(cfg_t*, production_t*);
static int cfg_atom(cfg_t*, production_t*);

static int
cfg_entry_section(cfg_t* cfg, int kind) {
	int (*section_ptr)(cfg_t*) = NULL;
	switch (kind) {
		case T_EXTERN:		section_ptr = &cfg_path_list;
					break;
		case T_ALIAS:		section_ptr = &cfg_alias_list;
					break;
		case T_PRODUCTION:	section_ptr = &cfg_production_list;
					break;
	}
	if (!section_ptr) {
		/* ERROR */
		return (ERROR);
	}
	if (advance_token(cfg->lex) != kind
			|| advance_token(cfg->lex) != T_LBRACE
			|| (*section_ptr)(cfg) == ERROR
			|| advance_token(cfg->lex) != T_RBRACE
			|| advance_token(cfg->lex) != T_SEMI) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_section(cfg_t* cfg) {
	bool empty = true;
	while (peek_token(cfg->lex) != T_EOF) {
		if (cfg_entry_section(cfg, peek_token(cfg->lex)) == ERROR)
			{ return (ERROR); }
		empty = false;
	}
	if (empty) {
		/* WARNING */
	}
	return (DONE);
}

int
cfg_path_list(cfg_t* cfg) {
	if (peek_token(cfg->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(cfg->lex) == T_LITERAL) {
		unget_c_buffer(LAST_LEXEME(cfg->lex), 1);
		PUSH_BACK_VECTOR(cfg->token_file, strdup(C_LEXEME(cfg->lex) + 1));
		if (peek_token(cfg->lex) == T_COMMA) {
			advance_token(cfg->lex);
			return (cfg_path_list(cfg));
		}
	}
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_alias_list(cfg_t* cfg) {
	if (peek_token(cfg->lex) == T_RBRACE)
		{ return (DONE); }
	else if (advance_token(cfg->lex) == T_LPAREN) {
		if (advance_token(cfg->lex) != T_GLOBAL_TOK) {
			/* ERROR */
			return (ERROR);
		}

		symbol_t* alias_ter = add_symbol_cfg(cfg, TERMINAL, C_LEXEME(cfg->lex));
		size_t index_alias = alias_ter->index;

//		alias_ter->is_defined = false;

		if (advance_token(cfg->lex) != T_BARROW
				|| advance_token(cfg->lex) != T_LITERAL) {
			/* ERROR */
			return (ERROR);
		}
		symbol_t* literal = add_symbol_cfg(cfg,
					LITERAL, C_LEXEME(cfg->lex));
		if (literal->terminal_alias != -1) {
			/* ERROR */
			fprintf(stderr, "Redefinning literal %s.\n",
					C_LEXEME(cfg->lex));
			return (ERROR);
		}
		literal->is_defined = true;
		literal->terminal_alias = index_alias;
		if (advance_token(cfg->lex) != T_RPAREN) {
			/* ERROR */
			return (ERROR);
		}
		if (peek_token(cfg->lex) == T_COMMA) {
			advance_token(cfg->lex);
			return (cfg_alias_list(cfg));
		}
	}
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_production_list(cfg_t* cfg) {
	while (peek_token(cfg->lex) != T_RBRACE) {
		if (cfg_production(cfg) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

static int
follow_prod(cfg_t* cfg) {
	symbol_t* symbol_lhs = add_symbol_cfg(cfg, NON_TERMINAL,
													C_LEXEME(cfg->lex));
	if (peek_token(cfg->lex) == T_ARROW) {
		advance_token(cfg->lex);
		symbol_lhs->is_defined = true;
		if (cfg_rhs(cfg, symbol_lhs) == ERROR) {
			/* ERROR */
			return (ERROR);
		}
	}
	else if (advance_token(cfg->lex) == T_EQUAL) {
		if (advance_token(cfg->lex) != T_START) {
			fprintf(stderr, "Must $START follow %s = .\n",
				symbol_lhs->name);
			return (ERROR);
		}
		cfg->goal = symbol_lhs->index;
	}
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_production(cfg_t* cfg) {
	if (advance_token(cfg->lex) != NON_TERMINAL
			|| follow_prod(cfg) == ERROR
			|| advance_token(cfg->lex) != T_SEMI) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_rhs(cfg_t* cfg, symbol_t* lhs) {
	production_t* prod = new_production(lhs, SIZE_VECTOR(cfg->productions));
	if (!prod)
		{ return (ERROR); }
	if (cfg_opt_list(cfg, prod) == ERROR) {
		del_production(prod);
		return (ERROR);
	}

	ADD_BITSET(lhs->prod_lst, prod->index);
	PUSH_BACK_VECTOR(cfg->productions, prod);

	while (peek_token(cfg->lex) == T_UNION) {
		advance_token(cfg->lex);
		prod = new_production(lhs, SIZE_VECTOR(cfg->productions));
		if (!prod)
			{ return (ERROR); }
		if (cfg_opt_list(cfg, prod) == ERROR) {
			del_production(prod);
			return (ERROR);
		}

		ADD_BITSET(lhs->prod_lst, prod->index);
		PUSH_BACK_VECTOR(cfg->productions, prod);
	}
	return (DONE);
}

int
cfg_opt_list(cfg_t* cfg, production_t* prod) {
	if (in_first(cfg->lex, NON_TERMINAL, TERMINAL, LITERAL, -1))
		{ return (cfg_list(cfg, prod)); }
	else if (peek_token(cfg->lex) == T_EMPTY)
		{ advance_token(cfg->lex); }
	else if (!in_first(cfg->lex, T_UNION, T_SEMI, -1)) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_list(cfg_t* cfg, production_t* prod) {
	if (in_first(cfg->lex, NON_TERMINAL, TERMINAL, LITERAL, -1)) {
		if (cfg_atom(cfg, prod) == ERROR)
			{ return (ERROR); }
	}
	while (in_first(cfg->lex, NON_TERMINAL, TERMINAL, LITERAL, -1)) {
		if (cfg_atom(cfg, prod) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

int
cfg_atom(cfg_t* cfg, production_t* prod) {
	symbol_t* atom_symbol = add_symbol_cfg(cfg,
				advance_token(cfg->lex), C_LEXEME(cfg->lex));
	if ((atom_symbol->kind == LITERAL)
			&& (atom_symbol->terminal_alias != -1)) {
		add_symbol_rhs(prod, AT_VECTOR(cfg->terminal,
					atom_symbol->terminal_alias));
	}
	else
		{ add_symbol_rhs(prod, atom_symbol); }
	return (DONE);
}

static int
unused_symbol(vector_t const* symbol_tab) {
	int unused = DONE;
	for (size_t i = 0; i < SIZE_VECTOR(symbol_tab); ++i) {
		symbol_t* symbol = (symbol_t*)AT_VECTOR(symbol_tab, i);
		if (!symbol->is_defined) {
			fprintf(stderr, "%s %s used but not defined.\n",
				((IS_NON_TERMINAL(symbol))
					? "Non Terminal" : "Literal"), symbol->name);
			unused = ERROR;
		}
	}
	return (unused);
}

int
detect_bad_symbol(cfg_t* cfg) {
	if (!cfg)
		{ return (ERROR); }
	int exit_status = unused_symbol(cfg->non_terminal)
			|| unused_symbol(cfg->terminal);
	return (exit_status);
}

#ifdef PRINT_DEBUG

void
print_terminal(cfg_t const* cfg) {
	puts("=== TERMINAL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->terminal); ++i) {
		printf("%s\n", ((symbol_t*)
			AT_VECTOR(cfg->terminal, i))->name);
	}
}

void
print_non_terminal(cfg_t const* cfg) {
	puts("=== NON_TERMINAL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		printf("%s\n", ((symbol_t*)
			AT_VECTOR(cfg->non_terminal, i))->name);
	}
}

void
print_production(cfg_t const* cfg) {
	puts("=== PRODUCTION ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = AT_VECTOR(cfg->productions, i);
		printf("%s -> ", prod->symbol_lhs->name);
		list_rhs_t* list = prod->rhs_element;
		while (list) {
			printf("%s", list->symbol_rhs->name);
			if (list->next)
				{ printf(" "); }
			list = list->next;
		}
		puts("");
	}
}

void
print_nullable(cfg_t const* cfg) {
	puts("=== NULLABLE SYMBOL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* non_terminal = AT_VECTOR(cfg->non_terminal, i);
		if (non_terminal->nullable)
			{ printf("%s is nullable.\n", non_terminal->name); }
	}
}

#endif /* PRINT_DEBUG */

