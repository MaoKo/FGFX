#include <string.h>
#include <ctype.h>

#include "regex_node.h"
#include "regex.h"

regex_node_t*
new_regex_node(int kind, ...) {
    regex_node_t* node = NEW(regex_node_t, 1);
    if (!node)
        { return (NULL_NODE); }
    memset(node, 0, sizeof(*node));

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

        case AST_DOT:
            node->is_dotall = va_arg(args, int);
            break;

        case AST_STAR:
        case AST_QUES:
        case AST_PLUS:
            node->left = va_arg(args, regex_node_t*);
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
        else if (root->kind_ast == AST_DOT)
            { return (new_regex_node(AST_DOT, root->is_dotall)); }
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

        /* Check for '*', '?', '+' operator */
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
                root->left = new_regex_node(AST_CLASS,
                                                COMPL_BITSET(negate_lang));  
                root->kind_ast = AST_STAR;
                break;

            case AST_CLASS:
                root->left = new_regex_node(AST_CLASS, 
                                                COMPL_BITSET(root->class));
                root->kind_ast = AST_STAR;  
                break;

            case AST_QUES:
                

            case AST_UNION:
                invert_node_language(root->left);
                invert_node_language(root->right);

                root->kind_ast = AST_CONCAT;

                break;

            case AST_LOOK:
            case AST_CONCAT: ;
                regex_node_t** change_node = NULL_NODE;
                if (CHILD_NODE(root->left->kind_ast))
                    { change_node = &root->left->right; }
                else
                    { change_node = &root->left; }

                regex_node_t* new_node = cpy_regex_node(*change_node);

                invert_node_language(root->left);
                (*change_node) = new_regex_node(AST_UNION,
                                                    root->left, new_node);

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
        case AST_DOT:
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

        case AST_STAR:
        case AST_QUES:
        case AST_PLUS:
            if (remove_useless_epsilon(root->left)) {
                del_regex_node(root->left);
                root->kind_ast = AST_EPSILON;

                return (true);
            }
            else
                { return (false); }

        default:
            return (false);
    }
}

regex_node_t*
set_dotall(regex_node_t* root) {
    if (!root)
        { return (NULL_NODE); }
    else if (root->kind_ast == AST_DOT)
        { root->is_dotall = true; }
    return (root);
}

regex_node_t*
set_skipws(regex_node_t* root) {
    if (!root)
        { return (NULL_NODE); }
    else if ((root->kind_ast == AST_SYMBOL) && IS_SKIPWS(root->symbol))
        { root->kind_ast = AST_EPSILON; }
    return (root);
}

regex_node_t*
set_igcase(regex_node_t* root) {
    if (!root)
        { return (NULL_NODE); }
    else if ((root->kind_ast == AST_SYMBOL) || (root->kind_ast == AST_CLASS)) {
        if ((root->kind_ast == AST_SYMBOL) && isalpha(root->symbol)) {
            size_t back_symbol = root->symbol;
            root->kind_ast = AST_CLASS;
            root->class = new_bitset();

            ADD_BITSET(root->class, back_symbol);
        }

        if (root->kind_ast == AST_CLASS) {
            int i;
            while ((i = IT_NEXT(root->class)) != IT_NULL) {
                if (isalpha(i)) {
                    size_t target = (islower(i) ? toupper(i) : tolower(i));
                    ADD_BITSET(root->class, target);    
                }   
            } 
            IT_RESET(root->class);
        }
    }
    return (root);
}

regex_node_t*
set_option_ast(regex_node_t* root, regex_node_t* (*opt_ptr)(regex_node_t*)) {
    if (!root)
        { return (NULL_NODE); }
    else if (CHILD_NODE(root->kind_ast)) {
        if (CHILD_NODE(root->left->kind_ast))
            { root->left = set_option_ast(root->left, opt_ptr); }
        else
            { root->left = (*opt_ptr)(root->left); }

        if (root->right && CHILD_NODE(root->right->kind_ast))
            { root->right = set_option_ast(root->right, opt_ptr); }
        else
            { root->right = (*opt_ptr)(root->right); }
        return (root);
    }
    else
        { return ((*opt_ptr)(root)); }
}

#if 0

static bool
can_generate_epsilon(regex_node_t* root) {
    if (!root)
        { return (false); }
    switch (root->kind_ast) {
        case AST_EPSILON:
            return (true);

        case AST_SYMBOL:
        case AST_CLASS:
            return (false);

        case AST_UNION:
            return (can_generate_epsilon(root->left)
                                    || can_generate_epsilon(root->right));

        case AST_CONCAT:
            return (can_generate_epsilon(root->left)
                                    && can_generate_epsilon(root->right));

        default:
            return (false);
    }
}

static regex_node_t*
add_node_suffix(regex_node_t* root, regex_node_t* suffix) {
    if (!root)
        { return (NULL_NODE); }
    else if ((root->kind_ast == AST_UNION)
                                && (root->right->kind_ast == AST_EPSILON)) {
        del_regex_node(root->right);

        root->right = cpy_regex_node(suffix);
        root->kind_ast = AST_CONCAT;

        return (root);
    }
    else {
        root->left = add_node_suffix(root->left, root->right);
        if (can_generate_epsilon(root->right))
            { root->right = add_node_suffix(root->right, suffix); }

        root->kind_ast = AST_UNION;
    }
    return (NULL_NODE);
}

void
readjust_opt_ast(regex_node_t* root) {
    if ((!root) && (root->kind_ast != AST_CONCAT))
        { return; }

    regex_node_t* new_root = root;
    while (new_root->left->kind_ast == AST_CONCAT) {
        new_root = new_root->left;
        if (!can_generate_epsilon(new_root->right))
            { root = new_root; }
    }

    if (!can_generate_epsilon(root->left))
        { return; }
    add_node_suffix(root, root->right);
}

#endif
