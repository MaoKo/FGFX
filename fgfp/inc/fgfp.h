#ifndef FGFP_H
#define FGFP_H

#include <stddef.h>
#include "vector.h"

#define HASH_SIZE	10

typedef struct {
	size_t index_lhs;
	struct rhs {
		int kind;
		size_t index_symbol;
		struct rhs* next;
	} list_rhs;
} production_t;

typedef struct {
	vector_t* non_terminal[HASH_SIZE];
	vector_t* terminal[HASH_SIZE];
	vector_t* productions;
	size_t goal;
} cfg_t;

#endif /* FGFP_H */
