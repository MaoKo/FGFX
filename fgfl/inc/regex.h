#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>
#include <limits.h>

#include "fgfl.h"

#define MIN_ASCII		(0)
#define MAX_ASCII		(UCHAR_MAX + 1)

#define EPSILON			(MAX_ASCII + 1)

regex_node_t* build_regex(lexical_spec_t*, spec_entry_t*);
void del_regex_node(regex_node_t*);

#endif /* REGEX_H */
