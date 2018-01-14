#include "regex_node.h"
#include "regex.h"

regex_node_t*
new_regex_node(int kind, ...) {
	regex_node_t* node = NEW(regex_node_t, 1);
	if (!node)
		{ return (NULL); }
	va_list args;
	va_start(args, kind);
	if (kind != AST_SYMBOL) {
		node->left = va_arg(args, regex_node_t*);
		node->right = va_arg(args, regex_node_t*);
		node->look_sym = false;
	}
	else {
		node->alone = va_arg(args, int);
		if (node->alone)
			{ node->symbol = va_arg(args, int); }
		else
			{ node->cclass = va_arg(args, bitset_t*); }
	}
	node->kind_ast = kind;
	va_end(args);
	return (node);
}

void
del_regex_node(regex_node_t* node) {
	if (node) {
		if (node->kind_ast != AST_SYMBOL) {
			del_regex_node(node->left);
			del_regex_node(node->right);
		}
		else if (!node->alone)
			{ del_bitset(node->cclass); }
		FREE(node);
	}
}

regex_node_t*
cpy_regex_node(regex_node_t* root) {
	if (root) {
		if (root->kind_ast != AST_SYMBOL) {
			regex_node_t* left = cpy_regex_node(root->left);
			regex_node_t* right = cpy_regex_node(root->right);
			return (new_regex_node(root->kind_ast, left, right));
		}
		else { 
			if (root->alone) {
				return (new_regex_node(AST_SYMBOL,
						ALONE_S, root->symbol)); }
			else {
				return (new_regex_node(AST_SYMBOL, MULTI_S,
						dup_bitset(root->cclass)));
			}
		}
	}
	return (NULL);
}

regex_node_t*
cpy_concat_regex_node(regex_node_t* root, size_t size) {
	if (!size)
		{ return (new_regex_node(AST_SYMBOL, EPSILON)); }
	regex_node_t* atom = root;
	for (size_t i = 1; i < size; ++i)
		{ root = new_regex_node(AST_CONCAT, root, cpy_regex_node(atom)); }
	return (root);
}
