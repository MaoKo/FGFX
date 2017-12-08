#include <stdio.h>
#include <unistd.h>

#include "fgfp.h"
#include "output_ll.h"
#include "output.h"
#include "utils.h"

void
output_location_token(int filde, char const* path_token) {
	dprintf(filde, DEFINE(ONLY_TOKEN,) "\n");
	dprintf(filde, INCLUDE(%s) "\n", path_token);
	dprintf(filde, UNDEF(ONLY_TOKEN) "\n\n");
}

void
output_nter_symbol(int filde, char const* nter_str) {
	write(filde, "N", 1);
	for (size_t i = 1; nter_str[i] != '>'; ++i) {
		if (nter_str[i] == '\'')
			{ dprintf(filde, "_PRIME"); }
		else
			{ write(filde, nter_str + i, 1); }
	}
}

void
output_non_terminal_enum(int filde, vector_t const* nter) {
	dprintf(filde, ENUM " " BEG_BLOCK "\n");
	for (size_t i = 0; i < SIZE_VECTOR(nter) - 1; ++i) {
		dprintf(filde, TAB);
		output_nter_symbol(filde, ((symbol_t*)AT_VECTOR(nter, i))->name);
		dprintf(filde, ",\n");
	}
	dprintf(filde, END_BLOCK SEMI "\n\n");
	dprintf(filde, DEFINE(TOTAL_NTER, %zu)"\n\n", SIZE_VECTOR(nter));
}

void
output_ll_table(int filde, cfg_t const* cfg, vector_t const* ll_table) {
	dprintf(filde, STATIC " " INT " ll_table[TOTAL_NTER][%zu] = " BEG_BLOCK "\n",
			SIZE_VECTOR(cfg->terminal));
	for (size_t i = 0; i < SIZE_VECTOR(ll_table); ++i) {
		trans_list_t* list = AT_VECTOR(ll_table, i);
		if (list)
			{ dprintf(filde, "\t"); }
		while (list) {
			dprintf(filde, "[");
			output_nter_symbol(filde, ((symbol_t*)
				AT_VECTOR(cfg->non_terminal, i))->name);
			dprintf(filde, "][T%s]=%d, ", ((symbol_t*)
				AT_VECTOR(cfg->terminal, list->input))->name,
				list->state + 1);
			list = list->next;
		}
		write(filde, "\n", 1);
	}
	dprintf(filde, END_BLOCK SEMI "\n");
}

