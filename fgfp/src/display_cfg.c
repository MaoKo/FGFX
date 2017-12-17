#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "display_cfg.h"
#include "display_dfa.h"
#include "display.h"
#include "utils.h"

void
display_location_token(int filde, vector_t const* path_token) {
	dprintf(filde, DEFINE(ONLY_TOKEN,) NL);
	for (size_t i = 0; i < SIZE_VECTOR(path_token); ++i) {
		dprintf(filde, INCLUDE(%s) NL,
			(char const*)AT_VECTOR(path_token, i));
	}
	dprintf(filde, UNDEF(ONLY_TOKEN) NL NL);
}

void
display_nter_symbol(int filde, cfg_t const* cfg, size_t index) {
	vector_t const* nter = cfg->non_terminal;

	symbol_t* symbol = (symbol_t*)AT_VECTOR(nter, index);
	dprintf(filde, NTER_PREFIX);
	for (size_t i = 1; i < (strlen(symbol->name) - 1); ++i) {
		if (symbol->name[i] == '\'')
			{ dprintf(filde, PRIME_STR); }
		else
			{ write(filde, symbol->name + i, 1); }
	}
}

void
display_non_terminal_enum(int filde, cfg_t const* cfg) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		dprintf(filde, TAB);
		display_nter_symbol(filde, cfg, i);
		dprintf(filde, COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL,
			MACRO_NTER, SIZE_VECTOR(cfg->non_terminal) - 1);
}

void
display_nproduction_macro(int filde, cfg_t const* cfg, size_t index) {
	dprintf(filde, PROD_PREFIX);
	production_t* prod = (production_t*)AT_VECTOR(cfg->productions, index);
	display_nter_symbol(filde, cfg, prod->symbol_lhs->index);
	list_rhs* list = prod->rhs_element;
	while (list) {
		dprintf(filde, "_");
		if (IS_TERMINAL(list->symbol_rhs)) {
			dprintf(filde, TOKEN_PREFIX "%s",
				list->symbol_rhs->name);
		}
		else
			{ display_nter_symbol(filde, cfg, list->symbol_rhs->index); }
		list = list->next;
	}
}

void
display_production_macro(int filde, cfg_t const* cfg) {
	dprintf(filde, "//Various macro representing production." NL);
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions) - 1; ++i) {
		dprintf(filde, "#define" SP);
		display_nproduction_macro(filde, cfg, i);
		dprintf(filde, TAB "%zu" NL, i + 1);
	}
	dprintf(filde, NL NL);
}

