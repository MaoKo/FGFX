#include <string.h>
#include <ctype.h>

#include "preprocess_regex.h"
#include "regex.h"
#include "regex_node.h"
#include "lexical_spec.h"
#include "vector.h"
#include "bitset.h"
#include "error.h"
#include "utils.h"

static int
dependency_macro(lexical_spec_t* spec,
                                spec_entry_t* entry, bitset_t** set_macro) {
    if (!set_macro)
        { return (ERROR); }
    else if (entry->kind == T_KEYWORD)
        { return (DONE); }

    vector_t* depend_vect = new_vector();
    int exit_st = DONE;

    search_kind_regex_node(entry->regex_ast, AST_BOUND_NAME, depend_vect);
    for (size_t i = 0; i < SIZE_VECTOR(depend_vect); ++i) {
        regex_node_t* bound_node = (regex_node_t*)AT_VECTOR(depend_vect, i);

        bool find_dependence = false;
        int depend_not_token = -1;

        int index = get_index_vector(spec->entry_vect,
                                bound_node->bound_name, &cmp_token_entry);
        if (index != -1) {
            find_dependence = true;
            spec_entry_t* crt_entry = (spec_entry_t*)
                                AT_VECTOR(spec->entry_vect, index);

            if ((crt_entry->kind == T_KEYWORD))
                { depend_not_token = T_KEYWORD; }
            else {
                if (!*set_macro)
                    { *set_macro = new_bitset(); }
                ADD_BITSET(*set_macro, GET_INDEX(crt_entry));
                bound_node->index_token = index;

                if (crt_entry->use_look) {
                    warnf(0, "The token %s use lookahead operator. It's "
                            "feature is disable when expand.", crt_entry->name);
                }
            }
        }
        else {
            int index = get_index_vector(spec->state_vect,
                                            bound_node->bound_name, &strcmp);
            if (index != -1) {
                find_dependence = true;
                depend_not_token = T_STATE;
            }
        }

        if (!find_dependence || (depend_not_token != -1)) {
            if (!find_dependence) {
                errorf(0, "Macro name %s is not defined anywhere.",
                                bound_node->bound_name);
            }
            else if (depend_not_token != -1) {
                errorf(0, "Macro name %s is a %s.", bound_node->bound_name,
                        (depend_not_token == T_KEYWORD) ? "keyword" : "state" );
            }

            exit_st = ERROR;
            break;
        }
    }

    if (exit_st)
        { del_bitset(*set_macro); }
    del_vector(depend_vect);

    return (exit_st);
}

static int
recur_node_topsort(lexical_spec_t* spec, spec_entry_t* crt_entry,
            size_t index_entry, vector_t* stack_order, bitset_t* seen_reg) {

    crt_entry->is_used = true;
    if (IS_PRESENT(seen_reg, GET_INDEX(crt_entry))) {
        if (GET_INDEX(crt_entry) == index_entry) {
            errorf(0, "Stack overflow. Cycle detected for regex %s.",
                                            crt_entry->name);
            return (ERROR);
        }
        return (DONE);
    }
    ADD_BITSET(seen_reg, GET_INDEX(crt_entry));

    bitset_t* depend_node = NULL;
    if (dependency_macro(spec, crt_entry, &depend_node) == ERROR)
        { return (ERROR); }

    int i;
    int exit_st = DONE;

    while ((i = IT_NEXT(depend_node)) != IT_NULL) {
        spec_entry_t* new_entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
        if (recur_node_topsort(spec, new_entry,
                                index_entry, stack_order, seen_reg) == ERROR)
            { exit_st = ERROR; }
    }

    PUSH_BACK_VECTOR(stack_order, (void*)GET_INDEX(crt_entry));
    del_bitset(depend_node);

    return (exit_st);
}

vector_t*
topological_sort(lexical_spec_t* spec) {
    vector_t* stack_order = new_vector();
    bitset_t* seen_reg = new_bitset();

    int exit_st = DONE;
    for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
        spec_entry_t* crt_entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
        if (!((crt_entry->kind == T_TERMINAL) && (!crt_entry->is_frag)))
            { continue; }
        else if (IS_PRESENT(seen_reg, GET_INDEX(crt_entry)))
            { continue; }

        if (recur_node_topsort(spec, crt_entry,
                                i, stack_order, seen_reg) == ERROR)
            { exit_st = ERROR; }
    }

    if (exit_st == ERROR) {
        del_vector(stack_order);
        stack_order = NULL_VECT;
    }
    del_bitset(seen_reg);

    return (stack_order);
}

int
expand_macro_regex(lexical_spec_t* spec) {
    vector_t* stack_order;
    int exit_st = DONE;

    if ((stack_order = topological_sort(spec))) {
        for (size_t i = 0; i < SIZE_VECTOR(stack_order); ++i) {
            size_t j = (long)AT_VECTOR(stack_order, i);
            spec_entry_t* crt_entry = (spec_entry_t*)
                                            AT_VECTOR(spec->entry_vect, j);
            replace_bound_name_node(crt_entry->regex_ast, spec);
            remove_useless_epsilon(crt_entry->regex_ast);

#if 0
            if (IS_LOOK(crt_entry->regex_ast))
                { readjust_opt_ast(crt_entry->regex_ast->right); }
#endif
        }
    }
    else
        { exit_st = ERROR; }

    del_vector(stack_order);
    return (exit_st);
}
