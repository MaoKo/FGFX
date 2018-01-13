#ifndef FGFL_H
#define FGFL_H

#include <stddef.h>
#include <stdbool.h>

#include "vector.h"
#include "bitset.h"
#include "buffer.h"
#include "lexer.h"
#include "utils.h"

typedef struct regex_node_t {
	enum {
		AST_UNION, AST_CONCAT,
		AST_CLOSURE, AST_SYMBOL,
	} kind_ast;

	union {
		struct { // if AST_UNION | AST_CONCAT | AST_CLOSURE
			struct regex_node_t* left;
			struct regex_node_t* right;
			bool look_sym;
		};
		struct { // if AST_SYMBOL
			bool alone;
			union {
				bitset_t* cclass;
				int symbol;
			};
		};
	};
} regex_node_t;

typedef struct edge_t edge_t;

typedef struct state_t {
	size_t index;
	int final;
	bool beg_look;
	edge_t* trans;
	// if CCLASS
	bitset_t* class;
	struct state_t* out_class;
} state_t;

struct edge_t {
	int label;
	state_t* out_state;
	edge_t* next;
};

typedef struct {
	edge_t* tail;
	state_t* head;
} nfa_frag_t;

/*

typedef struct {
	bitset_t* set_state;
	size_t hash_state;
//	trans_list_t* edge;
	size_t group;
} dfa_state_t;

*/

typedef struct {
	size_t index;
	int kind;
	bool is_used;
	char* name;
	char* reg_str;
	union {
		struct { // if TERMINAL
			bool is_igcase;
			bool is_frag;

			bool use_look;
			bool skip;

			enum { REGEX, NFA } active;
			union {
				regex_node_t* reg;
				nfa_frag_t* frag;
			};

			int default_state;
			bool all_state;
			trans_list_t* state_begin_lst;
		};
		struct { // if KEYWORD & STATE
			struct { // if STATE
				state_t* st_master;
				bool is_defined;
				bool is_reach;
			};
			size_t count;
		};
	};
} spec_entry_t;

typedef struct {
	lexer_t* lex;
	vector_t* entry_vect;
	vector_t* state_vect;
	int start_state;
	bool miss_regex;
	state_t* master;
	// Table generated during the build of the DFA
	vector_t* trans;
	vector_t* final;
	vector_t* middle;
} lexical_spec_t;

#endif /* FGFL_H */
