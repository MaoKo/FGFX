#include <stdio.h>
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

            int new_kind = root->kind_ast;
            if (new_kind == AST_LOOK)
                { new_kind = AST_CONCAT; }

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
                                        AT_VECTOR(spec->entry_vect,
                                        root->left->index_token))->regex_ast;

            del_regex_node(root->left);
            root->left = cpy_regex_node(target_node);
        }

        /* Check for '*' operator */
        if (root->right && root->right->kind_ast == AST_BOUND_NAME) {
            regex_node_t* target_node = ((spec_entry_t*)
                                        AT_VECTOR(spec->entry_vect,
                                        root->right->index_token))->regex_ast;

            del_regex_node(root->right);
            root->right = cpy_regex_node(target_node);
        }

        replace_bound_name_node(root->left, spec);
        replace_bound_name_node(root->right, spec);
    }
    else if (root->kind_ast == AST_BOUND_NAME) {
        regex_node_t* target_node = cpy_regex_node(((spec_entry_t*)
                    AT_VECTOR(spec->entry_vect, root->index_token))->regex_ast);

        FREE(root->bound_name);
        memcpy(root, target_node, sizeof(*root));

        FREE(target_node);
    }
}

void
invert_node_language(regex_node_t* root) {
    if (root) {
        bitset_t* negate_lang = NULL_BITSET;

        switch (root->kind_ast) {
            case AST_SYMBOL: ;
                negate_lang = new_bitset();
                ADD_BITSET(negate_lang, (size_t)root->symbol);

                root->class = COMPL_BITSET(negate_lang);  
                root->kind_ast = AST_CLASS;
              
                break;

            case AST_CLASS:
                root->class = COMPL_BITSET(root->class);  
                break;

            case AST_UNION:
                invert_node_language(root->left);
                invert_node_language(root->right);

                root->kind_ast = AST_CONCAT;

                break;

            case AST_LOOK:
            case AST_CONCAT:
                negate_lang = new_bitset();
                
                // TODO
                // Invert the concat operator

                invert_node_language(root->right);
                
                break;

            default:
                break;
        }
    }
}

bool
remove_useless_epsilon(regex_node_t* root) {
    if (!root)
        { return (false); }

    switch (root->kind_ast) {
        case AST_SYMBOL:
        case AST_CLASS:
            return (false);

        case AST_EPSILON:
            return (true);

        case AST_UNION:
            if ((remove_useless_epsilon(root->left))
                    && (remove_useless_epsilon(root->right))) {
                del_regex_node(root->left);
                del_regex_node(root->right);

                root->kind_ast = AST_EPSILON;
                return (true);
            }
            else
                { return (false); }

        case AST_LOOK:
        case AST_CONCAT: ;
            bool left = remove_useless_epsilon(root->left);
            bool right = remove_useless_epsilon(root->right);

            if (left || right) {
                if (left)
                    { del_regex_node(root->left); }
                if (right)
                    { del_regex_node(root->right); }

                if (left && right) {
                    root->kind_ast = AST_EPSILON;
                    return (true);
                }
                else {
                    regex_node_t* src_node = NULL_NODE;
                    if (!left)
                        { src_node = root->left; }
                    else
                        { src_node = root->right; }
                    memcpy(root, src_node, sizeof(*src_node));
                    FREE(src_node);
                }
            }
            return (false);

        case AST_CLOSURE:
            if (remove_useless_epsilon(root->left)) {
                del_regex_node(root->left);
                root->kind_ast = AST_EPSILON;

                return (true);
            }
            
            return (false);

        default:
            return (false);
    }
}

