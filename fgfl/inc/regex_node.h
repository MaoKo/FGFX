#ifndef REGEX_NONE_H
#define REGEX_NODE_H

#include <stdarg.h>
#include "fgfl.h"

regex_node_t* new_regex_node(int, ...);
void del_regex_node(regex_node_t*);
regex_node_t* cpy_regex_node(regex_node_t*);
regex_node_t* cpy_concat_regex_node(regex_node_t*, size_t);

#endif /* REGEX_NODE_H */
