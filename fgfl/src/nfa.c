#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "token_spec.h"
#include "utils.h"
#include "bitset.h"
#include "nfa.h"

state_t* record_state[MAX_STATE] = {};
static size_t current_index = 0;

void
del_record(void) {
	for (size_t i = 0; i < current_index; ++i) {
		state_t* state = record_state[i];
		if (state->trans) {
			edge_t* it = state->trans;
			while (it) {
				edge_t* next = it;
				it = it->next;
				FREE(next);	
			}
		}
		if (state->class)
			{ del_bitset(state->class); }
		FREE(state);
		record_state[i] = NULL;
	}
}

state_t*
new_state(void) {
	state_t* state = NEW(state_t, 1);
	if (!state)
		{ return (NULL); }
	memset(state, 0, sizeof(state_t));
	state->index_state = current_index;
	record_state[current_index++] = state;
	return (state);
}

static nfa_frag_t* 
new_nfa_frag(int tail_label, state_t* tail_start, state_t* head) {
	nfa_frag_t* frag = NEW(nfa_frag_t, 1);
	if (!frag)
		{ return (NULL); }
	frag->tail = NEW(edge_t, 1);
	if (!frag->tail)
		{ return (NULL); }
	memset(frag->tail, 0, sizeof(edge_t));
	INIT_EDGE(frag->tail, tail_label, tail_start);
	frag->head = head;
	return (frag);
}

static int
attach_tail(state_t* state, ...) {
	va_list args;
	va_start(args, state);
	nfa_frag_t* frag = NULL;
	while ((frag = va_arg(args, nfa_frag_t*))) {
		edge_t* new_edge = NEW(edge_t, 1);
		if (!new_edge)
			{ return (-1);  }
		CPY_EDGE(new_edge, frag->tail);
		PUSH_EDGE(state, new_edge);
	}
	va_end(args);
	return (0);
}

static int
make_transition(state_t* start, int label, state_t* final) {
	edge_t* new_edge = NEW(edge_t, 1);
	if (!new_edge)
		{ return (-1); }
	INIT_EDGE(new_edge, label, final);
	PUSH_EDGE(start, new_edge);
	return (0);
}

static nfa_frag_t* dfs_ast(regex_node_t*);
static bool crt_igcase = false;

static inline nfa_frag_t*
ast_symbol(regex_node_t* root) {
	state_t* final_s = new_state();
	if ((root->alone && !crt_igcase)
			|| (root->alone && crt_igcase
			&& !isalpha(root->symbol)))
		{ return (new_nfa_frag(root->symbol, final_s, final_s)); }

	if (root->alone) {
		size_t symbol = root->symbol;
		root->cclass = new_bitset();
		ADD_BITSET(root->cclass, symbol);
		root->alone = false;
	}

	state_t* relay_s  = new_state();
	relay_s->class = dup_bitset(root->cclass);
	relay_s->out_class = final_s;

	if (crt_igcase) {
		int i;
		while ((i = IT_NEXT(root->cclass)) != IT_NULL) {
			if (isalpha(i)) {
				size_t target = islower(i)
					? toupper(i) : tolower(i);
				ADD_BITSET(relay_s->class, target);	
			}	
		}
		IT_RESET(root->cclass);
	}

	return (new_nfa_frag(EPSILON, relay_s, final_s));
}

static nfa_frag_t*
ast_union(regex_node_t* root) {
	state_t* start = new_state();
	nfa_frag_t* left = dfs_ast(root->left);
	nfa_frag_t* right = dfs_ast(root->right);	
	if (attach_tail(start, left, right, NULL))
		{ /* TODO ERROR */ }
	state_t* final = new_state();
	if (make_transition(left->head, EPSILON, final)
			|| make_transition(right->head, EPSILON, final))
		{ /* TODO ERROR */ }
	FREE_FRAG(left); FREE_FRAG(right);
	return (new_nfa_frag(EPSILON, start, final));
}

static nfa_frag_t*
ast_concat(regex_node_t* root) {
	nfa_frag_t* left = dfs_ast(root->left);
	nfa_frag_t* right = dfs_ast(root->right);
	if (attach_tail(left->head, right, NULL))
		{ /* TODO ERROR */ }
	left->head = right->head;
	FREE_FRAG(right);
	return (left);
}

static nfa_frag_t*
ast_closure(regex_node_t* root) {
	nfa_frag_t* child = dfs_ast(root->left);
	state_t* front_state = new_state();
	if (attach_tail(front_state, child, NULL))
		{ /* TODO ERROR */ }
	make_transition(child->head, EPSILON, front_state);
	FREE_FRAG(child);
	return (new_nfa_frag(EPSILON, front_state, front_state));
}

/* Depth First Search over the ast for constructing a sub-NFA */
static nfa_frag_t* 
dfs_ast(regex_node_t* root) {
	if (root) {
		switch (root->kind_ast) {
			case AST_UNION:		return (ast_union(root));
						break;
			case AST_CONCAT:	return (ast_concat(root));
						break;
			case AST_CLOSURE:	return (ast_closure(root));
						break;
			case AST_SYMBOL:	return (ast_symbol(root));
						break;
		}	
	}
	return (NULL);
}

nfa_frag_t*
ast_to_nfa(regex_node_t* root, int priority, bool igcase) {
	crt_igcase = igcase;
	nfa_frag_t* frag = dfs_ast(root);
	STATE_FINAL(frag->head, priority);
	return (frag);
}

int
nfa_gen(token_spec_t* spec) {
	if (!spec)
		{ return (-1); }
	spec->master = new_state();
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(spec->entry_lst, i);
		if (entry->kind == GLOBAL) {
			regex_node_t* ast = entry->reg;
			entry->frag = ast_to_nfa(ast, i + 1, entry->igcase);
			del_node_ast(ast);

			entry->phase = FRAGMENT;
			if (attach_tail(spec->master, entry->frag, NULL))
				{ return (-1); }
		}
	}
	return (0);
}

