#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include "cfg.h"
#include "bitset.h"
#include "cfg_production.h"
#include "cfg_set_op.h"
#include "ll.h"
#include "utils.h"

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

	printf("Start symbol = %s\n", ((symbol_t*)
					AT_VECTOR(cfg->non_terminal, cfg->goal)
					)->name);

	printf("%d\n", unreachable_production(cfg));

	augment_grammar(cfg);
	detect_nullable(cfg);
	compute_first(cfg);
	compute_follow(cfg);
	
	if (!is_ll1(cfg)) {
		printf("Is not LL(1) :(\n");
		del_cfg(cfg);
		exit(1);
	}

	vector_t* vect = gen_ll1_table(cfg);
	printf("static int ll_table[][%zu] = {\n", SIZE_VECTOR(cfg->terminal));
	for (size_t i = 0; i < SIZE_VECTOR(vect); ++i) {
		trans_list_t* lst = AT_VECTOR(vect, i);
		while (lst) {
			printf("\t[N%s][T%s]=%d,",
	((production_t*)AT_VECTOR(cfg->productions, i))->symbol_lhs->name,
	((symbol_t*)AT_VECTOR(cfg->terminal, lst->input))->name,
		lst->state);
			lst = lst->next;
		}
		puts("");
	}
	printf("}\n");

#if 0
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
		printf("FIRST(%s) = {", non_terminal->name);
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

	puts("=== FOLLOW SET ===");
	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* non_terminal = AT_VECTOR(cfg->non_terminal, i);
		printf("FOLLOW(%s) = {", non_terminal->name);
		bool first = true;
		int j;
		while ((j = IT_NEXT(non_terminal->follow)) != -1) {
			if (!first)
				{ printf(", "); }
			symbol_t* terminal = AT_VECTOR(cfg->terminal, j);
			printf("%s", terminal->name);
			first = false;
		}
		puts("}");
	}
#endif
	
	del_cfg(cfg);
	return (0);
}
