#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "fgfp.h"
#include "display_ll.h"
#include "display_dfa.h"
#include "ll.h"
#include "display.h"
#include "utils.h"

void
display_ll_useful_macro(int filde, cfg_t const* cfg) {
	dprintf(filde, DEFINE(ERROR_SLOT, 0) NL);
	dprintf(filde, DEFINE(START_SYMBOL,));
	production_t* start_prod = BACK_VECTOR(cfg->productions);
	display_nter_symbol(filde, cfg, start_prod->rhs_element->symbol_rhs->index);
	dprintf(filde, NL NL);
}

void
display_ll_table(int filde, cfg_t const* cfg,
				vector_t const* ll_table, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	display_verbatim_file(filde, header);
	dprintf(filde, "_ll_table[" MACRO_NTER "]["
				MACRO_TOKEN "] = " BEG_BLOCK NL);

	for (size_t i = 0; i < SIZE_VECTOR(ll_table); ++i) {
		trans_list_t* list = AT_VECTOR(ll_table, i);
		if (list)
			{ dprintf(filde, TAB); }
		while (list) {
			dprintf(filde, "[");
			display_nter_symbol(filde, cfg, i);
			dprintf(filde, "]["TOKEN_PREFIX"%s]=", ((symbol_t*)
				AT_VECTOR(cfg->terminal, list->input))->name);
			display_nproduction_macro(filde, cfg, list->state);
			dprintf(filde, COMMA SP);
			list = list->next;
		}
		write(filde, NL, 1);
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
display_synchronizing_token(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	display_verbatim_file(filde, header);

	dprintf(filde, "_sync_token[%s][%zu] = " BEG_BLOCK NL,
		MACRO_NTER, count_max_follow(cfg->non_terminal) + 1);

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = AT_VECTOR(cfg->non_terminal, i);
		dprintf(filde, TAB "[");
		display_nter_symbol(filde, cfg, i);
		dprintf(filde, "] = " BEG_BLOCK SP);
		int j;
		while ((j = IT_NEXT(nter->follow)) != -1) {
			symbol_t* terminal = AT_VECTOR(cfg->terminal, j);
			dprintf(filde, TOKEN_PREFIX "%s, ", terminal->name);
		}
		dprintf(filde, "-1" " " END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

int
display_ll_matrix(cfg_t const* cfg, char const* base_file) {
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);
	if (filde == -1)
		{ return (ERROR); }
	display_require_macro(filde, base_file);

	display_location_token(filde, cfg->token_file);
	display_non_terminal_enum(filde, cfg);	

	display_ll_useful_macro(filde, cfg);

	vector_t* ll1_table = gen_ll1_table(cfg);
	display_ll_table(filde, cfg, ll1_table, header);
	del_vector(ll1_table);

	display_synchronizing_token(filde, cfg, header);

	display_endif(filde, base_file);

	if (close(filde) == -1)
		{ return (ERROR); }

	FREE(header);
	return (DONE);	
}

