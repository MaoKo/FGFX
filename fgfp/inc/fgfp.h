#ifndef FGFP_H
#define FGFP_H

#include <stddef.h>
#include <stdbool.h>

#include "utils.h"
#include "vector.h"
#include "bitset.h"
#include "lexer.h"

typedef struct {
	int precedence;
	bool left;
	bool right;
} precedence_t;

typedef struct symbol_t {
	size_t index;
	int kind;
	char const* name;
	bool is_defined;
	bool is_used;
	union {
		// if TERMINAL && ALIAS
		struct {
			union {
				// if TERMINAL
				struct
					{ bool is_eof; };
				// if ALIAS
				struct
					{ int terminal_alias; };
			};
			precedence_t* prec;
		};
		// if NON_TERMINAL
		struct {
			bool nullable;
			bool realizable;
			bitset_t* first;
			bitset_t* follow;
			bitset_t* prod_lst;
			// (Optional & List) elements
			struct symbol_t* special;
			size_t depth;
			size_t spec_nth;
		};
	};
} symbol_t;

typedef struct list_rhs_t {
	struct list_rhs_t* next;
	symbol_t* symbol_rhs;
	size_t pos;
} list_rhs_t;

typedef struct {
	size_t index;
	symbol_t* symbol_lhs;
	list_rhs_t* rhs_element;
	bitset_t* select_set;
	bool can_get_rid;
	symbol_t* mimic_sym;
} production_t;

typedef struct {
	production_t const* prod;
	list_rhs_t const* dot_pos;
	bool is_final;
	vector_t* non_kernel_lr0;
} lr0_item_t;

typedef struct {
	size_t index;
	lr0_item_t* core;
	bitset_t* lookahead;
	vector_t* non_kernel_look;
} lr1_item_t;

typedef struct {
	bitset_t* kernel_item;
	symbol_t* rprefix;
	bool ref_lr0;
	vector_t* non_lr0_kernel_item;
	bitset_t* final_state;
} kernel_t;

typedef struct {
	bitset_t* items;
	size_t hash_items;
	trans_list_t* shift_lst;
	trans_list_t* goto_lst;
	trans_list_t* reduces;
	bool accept;
	trans_list_t** last_move;
	bool first_reach;
	bitset_t* equiv_state;
} lr1_state_t;

typedef struct {
	vector_t* productions;
	vector_t* non_terminal;
	vector_t* terminal;
	vector_t* token_file;
	lexer_t* lex;
	size_t goal;
	bool miss_prod;
} cfg_t;

#endif /* FGFP_H */
