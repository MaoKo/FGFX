#ifndef REGEX_NONE_H
#define REGEX_NODE_H

#include <stdlib.h>
#include <stdarg.h>

#include "fgfl.h"
#include "vector.h"

#define CHILD_NODE(k)   ((k == AST_CLOSURE) || (k == AST_CONCAT)\
                            || (k == AST_UNION) || (k == AST_LOOK))

#define NULL_NODE       (NULL)

regex_node_t* new_regex_node(int, ...);
void del_regex_node(regex_node_t*);
regex_node_t* cpy_regex_node(regex_node_t*);
regex_node_t* cpy_concat_regex_node(regex_node_t*, size_t);
void search_kind_regex_node(regex_node_t*, size_t, vector_t*);
void replace_bound_name_node(regex_node_t*, lexical_spec_t*);

#endif /* REGEX_NODE_H */
