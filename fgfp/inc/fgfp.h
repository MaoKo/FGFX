#ifndef FGFP_H
#define FGFP_H

#include <stddef.h>
#include <stdbool.h>

#include "utils.h"
#include "vector.h"
#include "bitset.h"
#include "lexer.h"

typedef struct {
	size_t index;
	int kind;
	char const* name;
	bool is_defined;
	union {
		// if TERMINAL
		struct {
			bool is_eof;
			/* TODO: prec, left, right, ... */
		};
		// if NON_TERMINAL
		struct {
			bool nullable;
			bitset_t* first;
			bitset_t* follow;
			bitset_t* prod_lst;
		};
		// if ALIAS
		struct {
			int terminal_alias;
		};
	};
} symbol_t;

typedef struct list_rhs {
	size_t pos;
	symbol_t* symbol_rhs;
	struct list_rhs* next;
} list_rhs;

typedef struct {
	size_t index;
	symbol_t* symbol_lhs;
	list_rhs* rhs_element;
	bitset_t* select_set;
	bool can_get_rid;
} production_t;

typedef struct {
	production_t const* prod;
	list_rhs const* dot_pos;
	bool is_final;
} lr0_item_t;

typedef struct {
	size_t index;
	lr0_item_t* base_item;
	bitset_t* lookahead;
} lr1_item_t;

typedef struct {
	bitset_t* items;
	size_t hash_items;
	trans_list_t* shift_lst;
	trans_list_t* goto_lst;
	trans_list_t* reduces;
	bool accept;
	trans_list_t** last_move;
	bool first_reach;
} lr1_state_t;

typedef struct {
	vector_t* productions;
	vector_t* non_terminal;
	vector_t* terminal;
	vector_t* token_file;
	lexer_t* lex;
	size_t goal;
} cfg_t;

#endif /* FGFP_H */
