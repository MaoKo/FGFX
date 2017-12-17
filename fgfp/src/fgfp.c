#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "bitset.h"
#include "cfg_production.h"
#include "cfg_set_op.h"
#include "ll.h"
#include "lr.h"
#include "output_ll.h"
#include "output.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	if (argc <= 1)
		{ exit(1); }
	int filde = open(argv[1], O_RDONLY);
	cfg_t* cfg = parse_cfg(filde);
	if (!cfg)
		{ exit(1); }

	if (!SIZE_VECTOR(cfg->token_file)) {
		fprintf(stderr, "At most one location"
				"of token must be defined.\n");
		exit(1);
	}
	
	augment_grammar(cfg);
	if (detect_bad_symbol(cfg)) {
		del_cfg(cfg);
		exit(1);
	}

	if (unreachable_production(cfg)) {
		fprintf(stderr, "Some unreachable nonterminal"
				"has been remove.\n");
	}

	preprocess_literal(cfg);
	detect_nullable(cfg);
	compute_first(cfg);

	for (size_t i = 0; i < SIZE_VECTOR(cfg->non_terminal); ++i) {
		symbol_t* symbol = AT_VECTOR(cfg->non_terminal, i);
		if (is_empty_bitset(symbol->first)) {
			fprintf(stderr, "Non terminal %s derive no string at all.\n",
				symbol->name);
			del_cfg(cfg);
			exit(1);
		}
	}

	compute_follow(cfg);
	
/*
	if (!is_ll1(cfg)) {
		printf("Is not LL(1) :(\n");
		del_cfg(cfg);
		exit(1);
	}
*/
	
	vector_t* lr1_states = gen_lr1_states(cfg);
	compute_reduce_op(cfg, lr1_states);

	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		printf("===== State %zu =====\n", i);
		lr1_state_t* state = AT_VECTOR(lr1_states, i);
		if (state->accept)
			{ printf("(Accept): "); }
		print_item(state->items);
		trans_list_t* list = state->edges;
		while (list) {
			if (_SHIFT & list->input) {
				printf("SHIFT(%s)",
((symbol_t*)AT_VECTOR(cfg->terminal, list->input ^ _SHIFT))->name);
			}
			else if (_GOTO & list->input) {
				printf("GOTO(%s)",
((symbol_t*)AT_VECTOR(cfg->non_terminal, list->input ^ _GOTO))->name);
			}
			printf(" into state %d.\n", list->state);
			list = list->next;
		}
		list = state->reduces;
		while (list) {
			printf("TOKEN(%s) => REDUCE(%d).\n",
((symbol_t*)AT_VECTOR(cfg->terminal, list->input))->name,
(_REDUCE ^ list->state) + 1
);
			list = list->next;
		}
	}

	return (0);	

	output_ll_matrix(cfg, get_filename(argv[1]));

#if 0
	print_terminal(cfg);
	print_non_terminal(cfg);
	print_production(cfg);

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
