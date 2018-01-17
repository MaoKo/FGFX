#include <string.h>

#include "regex_node.h"
#include "regex.h"

regex_node_t*
new_regex_node(int kind, ...) {
	regex_node_t* node = NEW(regex_node_t, 1);
	if (!node)
		{ return (NULL_NODE); }

	va_list args;
	va_start(args, kind);

    switch (kind) {
        case AST_SYMBOL:
			node->symbol = va_arg(args, int);
            break;

        case AST_CLASS:
			node->class = va_arg(args, bitset_t*);
            break;

        case AST_BOUND_NAME:
            node->bound_name = va_arg(args, char const*);
            node->index_token = -1;
            break;

        case AST_EPSILON:
            break;

        default:
	    	node->left = va_arg(args, regex_node_t*);
	    	node->right = va_arg(args, regex_node_t*);

	    	node->look_sym = false;
    }

	node->kind_ast = kind;

	va_end(args);
	return (node);
}

void
del_regex_node(regex_node_t* node) {
	if (node) {
        if (node->kind_ast == AST_CLASS) 
			{ del_bitset(node->class); }
   	    else if (node->kind_ast == AST_BOUND_NAME)
             { FREE(node->bound_name); }
		else if (CHILD_NODE(node->kind_ast)) {
			del_regex_node(node->left);
			del_regex_node(node->right);
		}
		FREE(node);
	}
}

regex_node_t*
cpy_regex_node(regex_node_t* root) {
	if (root) {
		if (root->kind_ast == AST_SYMBOL)
            { return (new_regex_node(AST_SYMBOL, root->symbol)); }
		else if (root->kind_ast == AST_EPSILON)
            { return (new_regex_node(AST_EPSILON)); }
        else if (root->kind_ast == AST_CLASS)
            { return (new_regex_node(AST_CLASS, dup_bitset(root->class))); }
        else if (root->kind_ast == AST_BOUND_NAME) {
            return (new_regex_node(root->kind_ast,
                                                strdup(root->bound_name)));
        }
		else if (CHILD_NODE(root->kind_ast)) { 
			regex_node_t* left = cpy_regex_node(root->left);
			regex_node_t* right = cpy_regex_node(root->right);

			return (new_regex_node(root->kind_ast, left, right));
		}
	}
	return (NULL_NODE);
}

regex_node_t*
cpy_concat_regex_node(regex_node_t* root, size_t size) {
	if (!size)
		{ return (new_regex_node(AST_EPSILON)); }
	regex_node_t* atom = root;
	for (size_t i = 1; i < size; ++i)
		{ root = new_regex_node(AST_CONCAT, root, cpy_regex_node(atom)); }
	return (root);
}

void
search_kind_regex_node(regex_node_t* root, size_t kind, vector_t* dst_vect) {
    if (!root)
        { return; }
    else if (root->kind_ast == kind)
        { PUSH_BACK_VECTOR(dst_vect, root); }
    else if (CHILD_NODE(root->kind_ast)) {
        search_kind_regex_node(root->left, kind, dst_vect);
        search_kind_regex_node(root->right, kind, dst_vect);
    }
}

void
replace_bound_name_node(regex_node_t* root, lexical_spec_t* spec) {
    if (!root)
        { return; }
    else if (CHILD_NODE(root->kind_ast)) {
        if (root->left->kind_ast == AST_BOUND_NAME) {
            regex_node_t* target_node = ((spec_entry_t*)
                    AT_VECTOR(spec->entry_vect, root->left->index_token))->reg;
            del_regex_node(root->left);
            root->left = cpy_regex_node(target_node);
        }
        /* Check for '*' operator */
        if (root->right && root->right->kind_ast == AST_BOUND_NAME) {
            regex_node_t* target_node = ((spec_entry_t*)
                    AT_VECTOR(spec->entry_vect, root->right->index_token))->reg;
            del_regex_node(root->right);
            root->right = cpy_regex_node(target_node);
        }

        replace_bound_name_node(root->left, spec);
        replace_bound_name_node(root->right, spec);
    }
    else if (root->kind_ast == AST_BOUND_NAME) {
        regex_node_t* target_node = cpy_regex_node(((spec_entry_t*)
                         AT_VECTOR(spec->entry_vect, root->index_token))->reg);
        FREE(root->bound_name);
        memcpy(root, target_node, sizeof(*root));
        FREE(target_node);
    }
}
