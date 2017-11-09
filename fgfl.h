#ifndef FGFL_H
#define FGFL_H

#include "vector.h"
#include "set.h"

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
				set_t* cclass;
				int symbol;
			};
		};
	};
} node_ast_t;

typedef struct {
	int final;
	set_t* edges;
} state_t;

typedef struct {
	int symbol;
	state_t* out_state;
} edge_t;

typedef struct {
	edge_t* tail;
	state_t* head;
} nfa_fragment_t;

typedef struct {
	bool local;
	char* name;
	node_ast_t* reg;
	nfa_fragment_t* frag;	
} token_entry_t;

typedef struct {
	int filde;
	vector_t* entry_lst;
	state_t* master;
} token_spec_t;

#endif /* FGFL_H */
