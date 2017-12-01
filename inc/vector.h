#ifndef VECTOR_H
#define VECTOR_H

#include "utils.h"

#define AT_VECTOR(vect, i)	((vect)->body[i])
#define SET_VECTOR(vect, i, x)	((vect)->body[i] = x)
#define FRONT_VECTOR(vect)	((*vect)->body)
#define BACK_VECTOR(vect)	((vect)->body[vect->index - 1])
#define EMPTY_VECTOR(vect)	(!(vect)->index)
#define SIZE_VECTOR(vect)	((vect)->index)

typedef struct {
	void** body;
	size_t alloc;
	size_t index;
} vector_t;

vector_t* new_vector(void);
vector_t* dup_vector(vector_t const*);
void del_vector(vector_t*);
void clear_vector(vector_t*);
void insert_vector(vector_t*, size_t, void*);
void erase_vector(vector_t*, size_t);
void push_back_vector(vector_t*, void*);
void* pop_back_vector(vector_t*);
void reverse_vector(vector_t*);
int get_index_vector(vector_t*, void*);

#endif /* VECTOR_H */
