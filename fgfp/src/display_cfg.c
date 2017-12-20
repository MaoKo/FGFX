#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "display_cfg.h"
#include "lr.h"
#include "display_lr.h"
#include "ll.h"
#include "display_ll.h"

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
display_nter_symbol(int filde, cfg_t const* cfg, size_t index, bool sep) {
	vector_t const* nter = cfg->non_terminal;

	symbol_t* symbol = (symbol_t*)AT_VECTOR(nter, index);
	dprintf(filde, NTER_PREFIX);
	if (sep)
		{ dprintf(filde, SEP); }
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
		display_nter_symbol(filde, cfg, i, true);
		dprintf(filde, COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL,
			MACRO_NTER, SIZE_VECTOR(cfg->non_terminal) - 1);
}

void
display_nproduction_macro(int filde, cfg_t const* cfg, size_t index) {
	dprintf(filde, PROD_PREFIX SEP);
	production_t* prod = (production_t*)AT_VECTOR(cfg->productions, index);
	display_nter_symbol(filde, cfg, prod->symbol_lhs->index, false);
	list_rhs* list = prod->rhs_element;
	while (list) {
		dprintf(filde, SEP);
		if (IS_TERMINAL(list->symbol_rhs))
			{ dprintf(filde, TOKEN_PREFIX "%s", list->symbol_rhs->name); }
		else
			{ display_nter_symbol(filde, cfg, list->symbol_rhs->index, false); }
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
display_synchronizing_token(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	display_verbatim_file(filde, header);

	dprintf(filde, "_sync_token[%s][%zu] = " BEG_BLOCK NL,
		MACRO_NTER, count_max_follow(cfg->non_terminal) + 1);

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = AT_VECTOR(cfg->non_terminal, i);
		dprintf(filde, TAB "[");
		display_nter_symbol(filde, cfg, i, true);
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

int
display_parsing_table(cfg_t const* cfg, char const* base_file) {
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);
	if (filde == -1)
		{ return (ERROR); }
	display_require_macro(filde, base_file);

	display_location_token(filde, cfg->token_file);
	display_non_terminal_enum(filde, cfg);	

	display_production_macro(filde, cfg);
	
	display_ll_useful_macro(filde, cfg);

	vector_t* ll1_table = gen_ll1_table(cfg);
	display_ll_table(filde, cfg, ll1_table, header);
	del_vector(ll1_table);

	dprintf(filde, "#if 0" NL);
	vector_t* lr1_states = gen_lr1_states(cfg);
	compute_reduce_op(cfg, lr1_states);

	display_action_enum(filde);
	display_lr_useful_macro(filde);
	display_action_table(filde, cfg, lr1_states);
	display_goto_table(filde, cfg, lr1_states);
	dprintf(filde, "#endif" NL);

	del_vector(lr1_states);

	display_synchronizing_token(filde, cfg, header);
	display_endif(filde, base_file);

	if (close(filde) == -1)
		{ return (ERROR); }

	FREE(header);
	return (DONE);	
}

