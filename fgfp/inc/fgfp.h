#ifndef FGFP_H
#define FGFP_H

#include <stddef.h>
#include <stdbool.h>

#include "vector.h"

#define HASH_SIZE	10

typedef struct {
	int kind;
	char const* name;
	union {
		// if TERMINAL
		struct {
			/* TODO: prec, left, right, ... */
		};
		struct
			{ bool is_defined; };
	};
} symbol_t;

typedef struct {
	symbol_t* symbol_lhs;
	struct list_rhs {
		symbol_t* symbol_rhs;
		struct list_rhs* next;
	} *rhs;
} production_t;

typedef struct {
	vector_t* productions;
/*
	vector_t* non_terminal[HASH_SIZE];
	vector_t* terminal[HASH_SIZE];
*/
	vector_t* non_terminal;
	vector_t* terminal;
	size_t goal;
} cfg_t;

#endif /* FGFP_H */
