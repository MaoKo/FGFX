#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "fgfp.h"
#include "output_ll.h"
#include "ll.h"
#include "output.h"
#include "utils.h"

void
output_location_token(int filde, vector_t const* path_token) {
	dprintf(filde, DEFINE(ONLY_TOKEN,) "\n");
	for (size_t i = 0; i < SIZE_VECTOR(path_token); ++i) {
		dprintf(filde, INCLUDE(%s) "\n", 
				(char const*)AT_VECTOR(path_token, i));
	}
	dprintf(filde, UNDEF(ONLY_TOKEN) "\n\n");
}

void
output_nter_symbol(int filde, vector_t const* nter, size_t index) {
	symbol_t* symbol = (symbol_t*)AT_VECTOR(nter, index);
	dprintf(filde, NTER_PREFIX);
	for (size_t i = 1; i < (strlen(symbol->name) - 1); ++i) {
		if (symbol->name[i] == '\'')
			{ dprintf(filde, "_PRIME"); }
		else
			{ write(filde, symbol->name + i, 1); }
	}
}

void
output_non_terminal_enum(int filde, vector_t const* nter) {
	dprintf(filde, ENUM " " BEG_BLOCK "\n");
	for (size_t i = 0; i < SIZE_VECTOR(nter) - 1; ++i) {
		dprintf(filde, TAB);
		output_nter_symbol(filde, nter, i);
		dprintf(filde, ",\n");
	}
	dprintf(filde, END_BLOCK SEMI "\n\n");
	dprintf(filde, DEFINE(%s, %zu)"\n\n",
			MACRO_NTER, SIZE_VECTOR(nter) - 1);
}

void
output_ll_useful_macro(int filde, cfg_t const* cfg) {
	dprintf(filde, DEFINE(ERROR_SLOT, 0)"\n");
	dprintf(filde, DEFINE(START_SYMBOL,));
	output_nter_symbol(filde, cfg->non_terminal, cfg->goal);
	dprintf(filde, "\n\n");
}

void
output_ll_table(int filde, cfg_t const* cfg,
				vector_t const* ll_table, char const* header) {
	dprintf(filde, STATIC " " INT "\n");
	output_verbatim_file(filde, header);
	dprintf(filde, "_ll_table[" MACRO_NTER "]["
				MACRO_TOKEN "] = " BEG_BLOCK "\n");

	for (size_t i = 0; i < SIZE_VECTOR(ll_table); ++i) {
		trans_list_t* list = AT_VECTOR(ll_table, i);
		if (list)
			{ dprintf(filde, "\t"); }
		while (list) {
			dprintf(filde, "[");
			output_nter_symbol(filde, cfg->non_terminal, i);
			dprintf(filde, "]["TOKEN_PREFIX"%s]=%d, ", ((symbol_t*)
				AT_VECTOR(cfg->terminal, list->input))->name,
				list->state + 1);
			list = list->next;
		}
		write(filde, "\n", 1);
	}
	dprintf(filde, END_BLOCK SEMI "\n\n");
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
output_synchronizing_token(int filde, cfg_t const* cfg, char const* header) {
	dprintf(filde, STATIC " " INT "\n");
	output_verbatim_file(filde, header);

	dprintf(filde, "_sync_token[%s][%zu] = " BEG_BLOCK "\n",
		MACRO_NTER, count_max_follow(cfg->non_terminal) + 1);

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal) - 1; ++i) {
		symbol_t* nter = AT_VECTOR(cfg->non_terminal, i);
		dprintf(filde, TAB "[");
		output_nter_symbol(filde, cfg->non_terminal, i);
		dprintf(filde, "] = " BEG_BLOCK " ");
		int j;
		while ((j = IT_NEXT(nter->follow)) != -1) {
			symbol_t* terminal = AT_VECTOR(cfg->terminal, j);
			dprintf(filde, TOKEN_PREFIX "%s, ", terminal->name);
		}
		dprintf(filde, "-1" " " END_BLOCK COMMA "\n");
	}
	dprintf(filde, END_BLOCK SEMI "\n\n");
}

int
output_ll_matrix(cfg_t const* cfg, char const* base_file) {
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);
	if (filde == -1)
		{ return (ERROR); }
	output_require_macro(filde, base_file);

	output_location_token(filde, cfg->token_file);
	output_non_terminal_enum(filde, cfg->non_terminal);	

	output_ll_useful_macro(filde, cfg);

	vector_t* ll1_table = gen_ll1_table(cfg);
	output_ll_table(filde, cfg, ll1_table, header);
	del_vector(ll1_table);

	output_synchronizing_token(filde, cfg, header);

	output_endif(filde, base_file);

	if (close(filde) == -1)
		{ return (ERROR); }

	FREE(header);
	return (DONE);	
}

