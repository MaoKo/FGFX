#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "cfg.h"
#include "bitset.h"
#include "cfg_production.h"

int main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(1); }
	int filde = open(argv[1], O_RDONLY);
	cfg_t* cfg = parse_cfg(filde);
	if (!cfg)
		{ exit(1); }

	if (detect_bad_symbol(cfg)) {
		del_cfg(cfg);
		exit(1);
	}

	detect_nullable(cfg);
	compute_first(cfg);

	puts("=== NON_TERMINAL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		printf("%s\n", ((symbol_t*)
			AT_VECTOR(cfg->non_terminal, i))->name);
	}

	puts("=== TERMINAL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->terminal); ++i) {
		printf("%s\n", ((symbol_t*)
			AT_VECTOR(cfg->terminal, i))->name);
	}
	
	puts("=== PRODUCTION ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->productions); ++i) {
		production_t* prod = AT_VECTOR(cfg->productions, i);
		printf("%s -> ", prod->symbol_lhs->name);
		list_rhs* list = prod->rhs_element;
		while (list) {
			printf("%s", list->symbol_rhs->name);
			if (list->next)
				{ printf(" "); }
			list = list->next;
		}
		puts("");
	}

	puts("=== NULLABLE SYMBOL ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* non_terminal = AT_VECTOR(cfg->non_terminal, i);
		if (non_terminal->nullable)
			{ printf("%s is nullable.\n", non_terminal->name); }
	}

	puts("=== FIRST SET ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* non_terminal = AT_VECTOR(cfg->non_terminal, i);
		printf("First(%s) = {", non_terminal->name);
		bool first = true;
		int j;
		while ((j = IT_NEXT(non_terminal->first)) != -1) {
			if (!first)
				{ printf(", "); }
			symbol_t* terminal = AT_VECTOR(cfg->terminal, j);
			printf("%s", terminal->name);
			first = false;
		}
		puts("}");
	}
	

	del_cfg(cfg);
	return (0);
}
