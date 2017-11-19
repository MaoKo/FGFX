#include "nfa.h"

vector_t* record_state = NULL;

void new_record(void) {
	if ((record_state = vector()))
		{ push_back_vector(record_state, NULL); }
}

void del_record(void) {
	for (size_t i = 0; i < size_vector(record_state); ++i) {
		state_t* crt_state = (state_t*)at_vector(record_state, i);
		if (crt_state) {
			del_set(crt_state->edges);
			FREE(crt_state);
		}
	}
	record_state = NULL;
}

state_t* state_at(size_t index) {
	return ((state_t*)at_vector(record_state, index));
} 

edge_t* edge(int symbol, int state) {
	edge_t* edg = NEW(edge_t, 1);
	if (!edg)
		{ return (NULL); }
	edg->symbol = symbol; edg->out_state = state;
	return (edg);
}

int state(int force_index) {
	state_t* st = NEW(state_t, 1);
	if (!st)
		{ return (-1); }
	st->edges = EMPTY_SET; st->final = 0;
	if (!record_state)
		{ new_record(); }
	int index;
	if (force_index == -1) {
		index = size_vector(record_state);
		push_back_vector(record_state, st);
	}
	else {
		index = force_index;
		set_vector(record_state, force_index, st);
	}
	return (index);
}

static nfa_fragment_t* nfa_fragment(edge_t* tail, int head) {
	nfa_fragment_t* frag = NEW(nfa_fragment_t, 1);
	if (!frag)
		{ return (NULL); }
	frag->tail = tail; frag->head = head;
	return (frag);
}

static nfa_fragment_t* dfs_ast(node_ast_t*);

static nfa_fragment_t* ast_symbol(node_ast_t* root) {
	int final_s = state(NEW_STATE);
	if (root->alone)
		{ return (nfa_fragment(edge(root->symbol, final_s), final_s)); }

	int relay_s  = state(NEW_STATE);
	set_t* it = root->cclass;
	while (it) {
		MAKE_TRANS(relay_s, (long)it->item, final_s);
		it = it->next;
	}
	return (nfa_fragment(edge(EPSILON, relay_s), final_s));
}

static nfa_fragment_t* ast_union(node_ast_t* root) {
	int start = state(NEW_STATE);

	nfa_fragment_t* left = dfs_ast(root->left);
	nfa_fragment_t* right = dfs_ast(root->right);
	
	ADD_EDGE2STATE(start, left->tail);
	ADD_EDGE2STATE(start, right->tail);

	int final = state(NEW_STATE);
	MAKE_TRANS(left->head, EPSILON, final);
	MAKE_TRANS(right->head, EPSILON, final);

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
	int front_state = state(NEW_STATE);
	ADD_EDGE2STATE(front_state, child->tail);
	MAKE_TRANS(child->head, EPSILON, front_state);
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

nfa_fragment_t* ast2nfa(node_ast_t* root, int tok) {
	nfa_fragment_t* frag = dfs_ast(root);
	MARK_STATE_FINAL(frag->head, tok);
	return (frag);
}

int merge_nfa_fragment(set_t* nfa_set) {
	int master = state(0);
	if (master == -1)
		{ return (-1); }
	set_t* it = nfa_set;
	while (it) {
		ADD_EDGE2STATE(master, ((nfa_fragment_t*)it->item)->tail);
		it = it->next;
	}
	del_set(nfa_set);
	return (master);
}

int NFAgen(token_spec_t* spec) {
	if (!spec)
		{ return (-1); }
	set_t* master = EMPTY_SET;
	for (size_t i = 0; i < size_vector(spec->entry_lst); ++i) {
		token_entry_t* entry = (token_entry_t*)
					at_vector(spec->entry_lst, i);
		if (!entry->local) {
			node_ast_t* ast = entry->reg;
			entry->frag = ast2nfa(ast, i + 1);
			del_node_ast(ast);

			entry->phase = FRAGMENT;
			add_set(&master, entry->frag);
		}
	}
	spec->master = merge_nfa_fragment(master);
	return (0);
}

