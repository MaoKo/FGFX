#ifndef VECTOR_H
#define VECTOR_H

#include "utils.h"

typedef struct {
	void** body;
	size_t alloc;
	size_t index;
} vector_t;

vector_t* vector(void);
vector_t* cpy_vector(vector_t const*);
void del_vector(vector_t*);
void* at_vector(vector_t const*, size_t);
void set_vector(vector_t*, size_t, void*);
void* front_vector(vector_t const*);
void* back_vector(vector_t const*);
bool empty_vector(vector_t const*);
size_t size_vector(vector_t const*);
void clear_vector(vector_t*);
void insert_vector(vector_t*, size_t, void*);
void erase_vector(vector_t*, size_t);
void push_back_vector(vector_t*, void*);
void* pop_back_vector(vector_t*);
void reverse_vector(vector_t*);
int get_index_vector(vector_t*, void*);

#endif /* VECTOR_H */
