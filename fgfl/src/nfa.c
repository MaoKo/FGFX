#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "lexical_spec.h"
#include "utils.h"
#include "bitset.h"
#include "nfa.h"

vector_t* record_nfa_state = NULL;

static void
del_state(state_t* state) {
	if (state) {
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
	}
	FREE(state);
}

void
del_nfa_record(void) {
	foreach_vector(record_nfa_state, &del_state);
	del_vector(record_nfa_state);
}

state_t*
new_state(void) {
	state_t* state = NEW(state_t, 1);

	if (!state)
		{ return (NULL); }
	memset(state, 0, sizeof(*state));

	if (!record_nfa_state)
		{ record_nfa_state = new_vector(); }

	state->index = SIZE_VECTOR(record_nfa_state);
	PUSH_BACK_VECTOR(record_nfa_state, state);

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
			{ return (ERROR);  }
		CPY_EDGE(new_edge, frag->tail);
		PUSH_EDGE(state, new_edge);
	}

	va_end(args);
	return (DONE);
}

static int
make_transition(state_t* start, int label, state_t* final) {
	edge_t* new_edge = NEW(edge_t, 1);
	if (!new_edge)
		{ return (ERROR); }
	memset(new_edge, 0, sizeof(*new_edge));

	INIT_EDGE(new_edge, label, final);
	PUSH_EDGE(start, new_edge);

	return (DONE);
}

static nfa_frag_t* dfs_ast(regex_node_t*);
static bool crt_igcase = false;

static inline nfa_frag_t*
ast_symbol(regex_node_t* root) {
	state_t* final_s = new_state();

	if ((root->alone && !crt_igcase)
				|| (root->alone && crt_igcase && !isalpha(root->symbol)))
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
				size_t target = (islower(i) ? toupper(i) : tolower(i));
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

	FREE_FRAG(left);
	FREE_FRAG(right);

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

int
ast_to_nfa(spec_entry_t* crt_entry) {
	crt_igcase = crt_entry->is_igcase;
	regex_node_t* root = crt_entry->reg;
	int exit_st = DONE;
	if ((crt_entry->frag = dfs_ast(root)) == NULL)
		{ exit_st = ERROR; }
	else {
		STATE_FINAL(crt_entry->frag->head, GET_INDEX(crt_entry) + 1);
		crt_entry->phase = FRAGMENT;
	}
	del_regex_node(root);
	return (exit_st);
}

int
build_nfa(lexical_spec_t* spec) {
	if (!spec)
		{ return (ERROR); }

	bool active_state = (spec->start_state != -1);
	if (!active_state)
		{ spec->master = new_state(); }

	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		if ((entry->kind != T_TERMINAL) || (entry->is_frag))
			{ continue; }
		else if (ast_to_nfa(entry) == ERROR)
			{ return (ERROR); }
		else if (!entry->is_frag) {
			if (!active_state) {
				if (attach_tail(spec->master, entry->frag, NULL) == ERROR)
					{ return (ERROR); }
			}
			else {
				int i;
				while ((i = IT_NEXT(entry->valid_state)) != IT_NULL) {
					spec_entry_t* crt_state = (spec_entry_t*)
									AT_VECTOR(spec->state_vect, (size_t)i);
					if (!crt_state->st_master)
						{ crt_state->st_master = new_state(); }

					if (attach_tail(crt_state->st_master,
											entry->frag, NULL) == ERROR)
						{ return (ERROR); }
				}
				IT_RESET(entry->valid_state);
			}
		}
	}

	return (DONE);
}
