#ifndef FGFL_H
#define FGFL_H

#include <stddef.h>
#include <stdbool.h>

#include "vector.h"
#include "bitset.h"
#include "buffer.h"
#include "lexer.h"

typedef struct node_ast_t {
	enum { AST_UNION, AST_CONCAT, AST_CLOSURE, AST_SYMBOL, } kind_ast;
	union {
		struct {
			struct node_ast_t* left;
			struct node_ast_t* right;
		};
		struct {
			bool alone;
			union {
				bitset_t* cclass;
				int symbol;
			};
		};
	};
} node_ast_t;

typedef struct edge_t edge_t;

typedef struct state_t {
	int final;
	unsigned int index_state;
	edge_t* trans;
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

typedef struct {
	size_t index;
	char* name;
	char* reg_str;
	int kind;
	bool used;
	union {
		struct { // if GLOBAL & LOCAL
			bool igcase;
			bool skip;
			enum { NONE, AST, FRAGMENT, } phase;
			union {
				node_ast_t* reg;
				nfa_frag_t* frag;
			};
		};
		struct // if KEYWORD
			{ size_t count; };
	};
} token_entry_t;

typedef struct {
	lexer_t* lex;
	vector_t* entry_lst;
	state_t* master;
} token_spec_t;

#endif /* FGFL_H */
