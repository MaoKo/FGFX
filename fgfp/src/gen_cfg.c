#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cfg_production.h"
#include "gen_cfg.h"

#include "gen_lr.h"
#include "gen_ll.h"

#include "gen_dfa.h"
#include "gen.h"
#include "utils.h"

void
gen_location_token(int filde, cfg_t const* cfg) {
	dprintf(filde, DEFINE(_ONLY_STATE_TOKEN_,) NL);
	for (size_t i = 0; i < SIZE_VECTOR(cfg->token_file); ++i) {
		dprintf(filde, INCLUDE(%s) NL, (char const*)
								AT_VECTOR(cfg->token_file, i));
	}
	dprintf(filde, UNDEF(_ONLY_STATE_TOKEN_) NL NL);
}

void
gen_nter_symbol(int filde, cfg_t const* cfg, size_t index, bool sep) {
	vector_t const* nter = cfg->non_terminal;
	symbol_t* symbol = (symbol_t*)AT_VECTOR(nter, index);

	size_t depth = 0;
	size_t spec_nth = 0;

	if (symbol->special) {
		dprintf(filde, ((symbol->kind == OPT) ? OPT_STR : LST_STR));
		if (sep)
			{ dprintf(filde, SEP); }
		
		depth = symbol->depth;
		spec_nth = symbol->spec_nth;

		symbol = symbol->special;
	}

	dprintf(filde, NTER_PREFIX);
	if (sep)
		{ dprintf(filde, SEP); }

	size_t len = strlen(symbol->name) - 1;
	for (size_t i = 1; i < len; ++i) {
		if (symbol->name[i] == '\'')
			{ dprintf(filde, PRIME_STR); }
		else
			{ write(filde, symbol->name + i, 1); }
	}
	if (depth)
		{ dprintf(filde, SEP "%zu" SEP "%zu", depth, spec_nth); }
}

void
gen_non_terminal_enum(int filde, cfg_t const* cfg) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		dprintf(filde, TAB);
		gen_nter_symbol(filde, cfg, i, true);
		dprintf(filde, COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL,
					MACRO_NTER, SIZE_VECTOR(cfg->non_terminal) - 1);
}

void
gen_nproduction(int filde, cfg_t const* cfg, size_t index) {
	dprintf(filde, PROD_PREFIX SEP LHS_STR SEP);

	production_t* prod = (production_t*)AT_VECTOR(cfg->productions, index);
	gen_nter_symbol(filde, cfg, prod->symbol_lhs->index, false);

	dprintf(filde, SEP RHS_STR);
	list_rhs_t* list = prod->rhs_element;
	while (list) {
		dprintf(filde, SEP);
		if (IS_TERMINAL(list->symbol_rhs))
			{ dprintf(filde, TOKEN_PREFIX "%s", list->symbol_rhs->name); }
		else
			{ gen_nter_symbol(filde, cfg, list->symbol_rhs->index, false); }
		list = list->next;
	}
}

void
gen_production_enum(int filde, cfg_t const* cfg) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions) - 1; ++i) {
		dprintf(filde, TAB);
		gen_nproduction(filde, cfg, i);
		dprintf(filde, COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(TOTAL_PROD, %zu) NL NL,
										SIZE_VECTOR(cfg->productions));
}

void
gen_rhs_table(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, "_rhs_prod_table[%s] = " BEG_BLOCK NL, "TOTAL_PROD");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions) - 1; ++i) {
		production_t* prod = (production_t*)AT_VECTOR(cfg->productions, i);
		dprintf(filde, TAB "[");
		gen_nproduction(filde, cfg, i);
		dprintf(filde, "] = %zu" COMMA NL,
								size_gen_list((gen_list_t*)prod->rhs_element));
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_lhs_table(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, "_lhs_prod_table[%s] = " BEG_BLOCK NL, "TOTAL_PROD");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions) - 1; ++i) {
		production_t* prod = (production_t*)AT_VECTOR(cfg->productions, i);
		dprintf(filde, TAB "[");
		gen_nproduction(filde, cfg, i);
		dprintf(filde, "] = ");
		gen_nter_symbol(filde, cfg, LHS(prod)->index, true);
		dprintf(filde, COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

static size_t
count_max_follow(vector_t const* vnter) {
	size_t max_follow = 0;
	for (size_t i = 0; i < SIZE_VECTOR(vnter) - 1; ++i) {
		symbol_t* nter = (symbol_t*)AT_VECTOR(vnter, i);

		size_t count_nter = count_elt_bitset(nter->follow);
		if (count_nter > max_follow)
			{ max_follow = count_nter; }
	}
	return (max_follow);
}

void
gen_synchronizing_token(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, "_sync_token[%s][%zu] = " BEG_BLOCK NL,
		MACRO_NTER, count_max_follow(cfg->non_terminal) + 1);

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = AT_VECTOR(cfg->non_terminal, i);
		dprintf(filde, TAB "[");
		gen_nter_symbol(filde, cfg, i, true);
		dprintf(filde, "] = " BEG_BLOCK SP);

		int j;
		while ((j = IT_NEXT(nter->follow)) != IT_NULL) {
			symbol_t* terminal = AT_VECTOR(cfg->terminal, j);
			dprintf(filde, TOKEN_PREFIX SEP "%s" COMMA SP, terminal->name);
		}
		dprintf(filde, "-1" SP END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

