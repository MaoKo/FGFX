#include "nfa.h"

#define MARK_STATE_FINAL(state, tok)	(state->final = tok)
#define ADD_EDGE2STATE(st, edg)		add_set(&st->edges, edg)

static edge_t* edge(int symbol, state_t* state) {
	edge_t* edg = NEW(edge_t, 1);
	if (!edg)
		{ return (NULL); }
	edg->symbol = symbol; edg->out_state = state;
	return (edg);
}

static state_t* state(void) {
	state_t* st = NEW(state_t, 1);
	if (!st)
		{ return (NULL); }
	st->edges = EMPTY_SET; st->final = 0;
	return (st);
}

static inline void make_trans(state_t* src, int symbol, state_t* dst) {
	edge_t* edg = edge(symbol, dst);
	ADD_EDGE2STATE(src, edg);
}

static nfa_fragment_t* nfa_fragment(edge_t* tail, state_t* head) {
	nfa_fragment_t* frag = NEW(nfa_fragment_t, 1);
	if (!frag)
		{ return (NULL); }
	frag->tail = tail; frag->head = head;
	return (frag);
}

static nfa_fragment_t* dfs_ast(node_ast_t*);

static nfa_fragment_t* ast_symbol(node_ast_t* root) {
	state_t* final_s = state();
	if (root->alone)
		{ return (nfa_fragment(edge(root->symbol, final_s), final_s)); }

	state_t* relay_s  = state();
	set_t* it = root->cclass;
	while (it) {
		make_trans(relay_s, (long)it->item, final_s);
		it = it->next;
	}
	return (nfa_fragment(edge(EPSILON, relay_s), final_s));
}

static nfa_fragment_t* ast_union(node_ast_t* root) {
	state_t* start = state();

	nfa_fragment_t* left = dfs_ast(root->left);
	nfa_fragment_t* right = dfs_ast(root->right);
	
	ADD_EDGE2STATE(start, left->tail);
	ADD_EDGE2STATE(start, right->tail);

	state_t* final = state();
	make_trans(left->head, EPSILON, final);
	make_trans(right->head, EPSILON, final);

	return (nfa_fragment(edge(EPSILON, start), final));
}

static nfa_fragment_t* ast_concat(node_ast_t* root) {
	nfa_fragment_t* left = dfs_ast(root->left);
	nfa_fragment_t* right = dfs_ast(root->right);
	ADD_EDGE2STATE(left->head, right->tail);
	return (nfa_fragment(left->tail, right->head));
}

static nfa_fragment_t* ast_closure(node_ast_t* root) {
	nfa_fragment_t* child = dfs_ast(root->left);
	state_t* front_state = state();
	ADD_EDGE2STATE(front_state, child->tail);
	make_trans(child->head, EPSILON, front_state);
	return (nfa_fragment(edge(EPSILON, front_state), front_state));
}

/* Depth First Search over the ast for constructing a sub-NFA */
static nfa_fragment_t* dfs_ast(node_ast_t* root) {
	if (root) {
		switch (root->kind_ast) {
			case AST_UNION:
				return (ast_union(root)); break;
			case AST_CONCAT:
				return (ast_concat(root)); break;
			case AST_CLOSURE:
				return (ast_closure(root)); break;
			case AST_SYMBOL:
				return (ast_symbol(root)); break;
		}	
	}
	return (NULL);
}

nfa_fragment_t* reg_ast2nfa(node_ast_t* root, int tok) {
	nfa_fragment_t* frag = dfs_ast(root);
	MARK_STATE_FINAL(frag->head, tok);
	return (frag);
}

state_t* merge_nfa_fragment(set_t* nfa_set) {
	state_t* master = state();
	if (!master)
		{ return (NULL); }
	set_t* it = nfa_set;
	while (it) {
		ADD_EDGE2STATE(master, ((nfa_fragment_t*)it->item)->tail);
		it = it->next;
	}
	del_set(nfa_set);
	return (master);
}
