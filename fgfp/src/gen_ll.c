#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "fgfp.h"
#include "gen_ll.h"
#include "gen_dfa.h"
#include "ll.h"
#include "gen.h"
#include "utils.h"

void
gen_ll_useful_macro(int filde, cfg_t const* cfg) {
	dprintf(filde, DEFINE(ERROR_SLOT, 0) NL);
	dprintf(filde, DEFINE(START_SYMBOL,));
	production_t* start_prod = BACK_VECTOR(cfg->productions);
	gen_nter_symbol(filde, cfg,
			start_prod->rhs_element->symbol_rhs->index, true);
	dprintf(filde, NL NL);
}

void
gen_ll_table(int filde, cfg_t const* cfg,
				vector_t const* ll_table, char const* header) {
	dprintf(filde, STATIC SP INT NL);
	gen_verbatim_file(filde, header);
	dprintf(filde, "_ll_table[" MACRO_NTER "]["
				MACRO_TOKEN "] = " BEG_BLOCK NL);

	for (size_t i = 0; i < SIZE_VECTOR(ll_table); ++i) {
		trans_list_t* list = AT_VECTOR(ll_table, i);
		if (list)
			{ dprintf(filde, TAB); }
		while (list) {
			dprintf(filde, "[");
			gen_nter_symbol(filde, cfg, i, true);
			dprintf(filde, "][" TOKEN_PREFIX SEP "%s]=", ((symbol_t*)
				AT_VECTOR(cfg->terminal, list->input))->name);
			gen_nproduction(filde, cfg, list->state);
			dprintf(filde, COMMA SP);
			list = list->next;
		}
		write(filde, NL, 1);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}
