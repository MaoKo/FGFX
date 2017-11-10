#ifndef SET_H
#define SET_H

#include "utils.h"

typedef struct set_t {
	void* item;
	struct set_t* next;
} set_t;

set_t* set(void*);
void del_set(set_t*);
set_t* cpy_set(set_t const*);
bool has_set(set_t const*, void*);
void add_set(set_t**, void*);
void union_set(set_t**, set_t const*);
set_t* intersection_set(set_t const*, set_t const*);
bool equal_set(set_t const*, set_t const*);
void del_item_set(set_t**, void*);
void print_set(set_t const*);
//TODO: set_t* power_set(set_t*);

#endif /* SET_H */
