#ifndef FGFP_H
#define FGFP_H

#include <stddef.h>
#include <stdbool.h>

#include "vector.h"
#include "bitset.h"

typedef struct {
	int kind;
	char const* name;
	size_t index;
	bool is_defined;
	union {
		// if TERMINAL
		struct {
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
	symbol_t* symbol_rhs;
	struct list_rhs* next;
} list_rhs;

typedef struct {
	symbol_t* symbol_lhs;
	list_rhs* rhs_element;
	bitset_t* select_set;
	bool can_get_rid;
} production_t;

typedef struct {
	vector_t* productions;
	vector_t* non_terminal;
	vector_t* terminal;
	size_t goal;
	char const* token_file;
} cfg_t;

#endif /* FGFP_H */
