#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "cfg_production.h"
#include "lexer.h"
#include "utils.h"

static lexer_t* lex = NULL;

cfg_t*
new_cfg(void) {
	cfg_t* cfg = NEW(cfg_t, 1);
	if (!cfg)
		{ return (NULL); }
	memset(cfg, 0, sizeof(cfg_t));
	cfg->productions = new_vector();
	cfg->non_terminal = new_vector();
	cfg->terminal = new_vector();
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
		if (cfg->token_file)
			{ FREE(cfg->token_file); }
	}
	FREE(cfg);
}


static int
cmp_symbol_name(symbol_t const* sym, char const* name) {
	return (strcmp(sym->name, name));
}

static symbol_t*
add_symbol_cfg(cfg_t* cfg, int kind, char const* crt_lexeme) {
	vector_t* dest = (kind == NON_TERMINAL) ? cfg->non_terminal : cfg->terminal;
	int index = get_index_vector(dest, crt_lexeme, &cmp_symbol_name);
	if (index != -1)
		{ return (AT_VECTOR(dest, index)); }
	symbol_t* symbol = NEW(symbol_t, 1);
	if (!symbol)
		{ return (NULL); }
	symbol->kind = kind;	
	symbol->name = strdup(crt_lexeme);
	symbol->prod_lst = new_bitset();

	if (!symbol->name || !symbol->prod_lst) {
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

static int cfg_syntax(cfg_t*);

cfg_t*
parse_cfg(int filde) {
	if (!(lex = new_lexer(filde)))
		{ return (NULL); }
	cfg_t* cfg = new_cfg();
	if (!cfg)
		{ return (NULL); }
	int exit_status = cfg_syntax(cfg);
	del_lexer(lex);
	if (exit_status == ERROR) {
		del_cfg(cfg);
		/* ERROR */
		return (NULL);
	}
	return (cfg);
}

void
augment_grammar(cfg_t* cfg) {
	if (!cfg)
		{ return; }
	symbol_t* start = add_symbol_cfg(cfg, NON_TERMINAL, "$start");
	production_t* prod = new_production(start);
	add_symbol_rhs(prod, AT_VECTOR(cfg->non_terminal, cfg->goal));
	add_symbol_rhs(prod, add_symbol_cfg(cfg, TERMINAL, "EOF"));
	cfg->goal = SIZE_VECTOR(cfg->productions);
	PUSH_BACK_VECTOR(cfg->productions, prod);
}

static int cfg_inst(cfg_t*);
static int cfg_directive(cfg_t*);
static int cfg_prod(cfg_t*);
static int cfg_rhs(cfg_t*, symbol_t* lhs);
static int cfg_opt_list(cfg_t*, production_t*);
static int cfg_list(cfg_t*, production_t*);
static int cfg_atom(cfg_t*, production_t*);

int
cfg_syntax(cfg_t* cfg) {
	if (cfg_inst(cfg) == ERROR)
		{ return (ERROR); }
	while (peek_token(lex) != TEOF) {
		if (cfg_inst(cfg) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

int
cfg_inst(cfg_t* cfg) {
	if (in_first(lex, TSTART, TTOKEN, -1)) {
		if (cfg_directive(cfg) == ERROR)
			{ return (ERROR); }
	}
	else if (cfg_prod(cfg) == ERROR)
		{ return (ERROR); }
	return (DONE);
}

int
cfg_directive(cfg_t* cfg) {
	if (!in_first(lex, TSTART, TTOKEN, -1)) {
		/* ERROR */
		return (ERROR);
	}
	if (advance_token(lex) == TSTART) {
		if (advance_token(lex) != NON_TERMINAL) {
			/* ERROR */
			return (ERROR);
		}
		cfg->goal = add_symbol_cfg(cfg, NON_TERMINAL, C_LEXEME(lex))->index;
	}
	else {
		if (advance_token(lex) != TSTR) {
			/* ERROR */
			return (ERROR);
		}
		if (cfg->token_file) {
			/* WARNING */
			FREE(cfg->token_file);
		}
		cfg->token_file = strdup(C_LEXEME(lex));
	}

	if (advance_token(lex) != TSEMI) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int cfg_prod(cfg_t* cfg) {
	if (advance_token(lex) != NON_TERMINAL) {
		/* ERROR */
		return (ERROR);
	}
	symbol_t* symbol_lhs = add_symbol_cfg(cfg, NON_TERMINAL, C_LEXEME(lex));
	symbol_lhs->is_defined = true;
	if ((advance_token(lex) != TARROW) || (cfg_rhs(cfg, symbol_lhs) == ERROR)
			|| (advance_token(lex) != TSEMI)) {
		free_symbol(symbol_lhs);
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_rhs(cfg_t* cfg, symbol_t* lhs) {
	production_t* prod = new_production(lhs);
	if (!prod)
		{ return (ERROR); }
	if (cfg_opt_list(cfg, prod) == ERROR) {
		del_production(prod);
		return (ERROR);
	}

	ADD_BITSET(lhs->prod_lst, SIZE_VECTOR(cfg->productions));
	PUSH_BACK_VECTOR(cfg->productions, prod);

	while (peek_token(lex) == TUNION) {
		advance_token(lex);
		prod = new_production(lhs);
		if (!prod)
			{ return (ERROR); }
		if (cfg_opt_list(cfg, prod) == ERROR) {
			del_production(prod);
			return (ERROR);
		}

		ADD_BITSET(lhs->prod_lst, SIZE_VECTOR(cfg->productions));
		PUSH_BACK_VECTOR(cfg->productions, prod);
	}
	return (DONE);
}

int
cfg_opt_list(cfg_t* cfg, production_t* prod) {
	if (in_first(lex, NON_TERMINAL, TERMINAL, -1))
		{ return (cfg_list(cfg, prod)); }
	else if (!in_first(lex, TUNION, TSEMI, -1)) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_list(cfg_t* cfg, production_t* prod) {
	if (in_first(lex, NON_TERMINAL, TERMINAL, -1)) {
		if (cfg_atom(cfg, prod) == ERROR)
			{ return (ERROR); }
	}
	while (in_first(lex, NON_TERMINAL, TERMINAL, -1)) {
		if (cfg_atom(cfg, prod) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

int
cfg_atom(cfg_t* cfg, production_t* prod) {
	if (in_first(lex, NON_TERMINAL, TERMINAL, -1)) {
		add_symbol_rhs(prod, add_symbol_cfg(cfg,
			advance_token(lex), C_LEXEME(lex)));
	}
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
detect_bad_symbol(cfg_t* cfg) {
	if (!cfg)
		{ return (ERROR); }
	size_t count_error = 0;
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* symbol = (symbol_t*)AT_VECTOR(cfg->non_terminal, i);
		if (!symbol->is_defined) {
			fprintf(stderr, "Non_Terminal %s used but no defined.\n",
				symbol->name);
			++count_error;
		}
	}
	return (count_error);
}
