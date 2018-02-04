#ifndef REGEX_NONE_H
#define REGEX_NODE_H

#include <stdlib.h>
#include <stdarg.h>

#include "fgfl.h"
#include "vector.h"

#define IS_LOOK(root)   ((root)->kind_ast == AST_LOOK)
#define IS_SKIPWS(c)    ((c == ' ') || (c == '\t') || (c == '\n'))

#define IS_CLOSURE(k)   ((k == AST_STAR) || (k == AST_QUES) || (k == AST_PLUS))

#define CHILD_NODE(k)   (IS_CLOSURE(k) || (k == AST_CONCAT)\
                            || (k == AST_UNION) || (k == AST_LOOK)\
                            || (k == AST_OPTION) || (k == AST_TILDE))

#define NULL_NODE       (NULL)

regex_node_t* new_regex_node(int, ...);
void del_regex_node(regex_node_t*);
regex_node_t* cpy_regex_node(regex_node_t*);
regex_node_t* cpy_concat_regex_node(regex_node_t*, size_t);
void search_kind_regex_node(regex_node_t*, size_t, vector_t*);
void replace_bound_name_node(regex_node_t*, lexical_spec_t*);
bool remove_useless_epsilon(regex_node_t*);

regex_node_t* set_dotall(regex_node_t*);
regex_node_t* set_skipws(regex_node_t*);
regex_node_t* set_igcase(regex_node_t*);

regex_node_t* set_option_ast(regex_node_t*, regex_node_t* (*)(regex_node_t*));

regex_node_t* reverse_regex_concat(regex_node_t*);

#endif /* REGEX_NODE_H */
