#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

#define AT_VECTOR(vect, i)		((vect)->body[i])
#define SET_VECTOR(vect, i, x)		((vect)->body[i] = x)
#define FRONT_VECTOR(vect)		((*vect)->body)
#define BACK_VECTOR(vect)		((vect)->body[(vect)->index - 1])
#define EMPTY_VECTOR(vect)		(!(vect)->index)
#define SIZE_VECTOR(vect)		((vect)->index)

#define PUSH_FRONT_VECTOR(vect, x)	(insert_vector(vect, 0, x))
#define PUSH_BACK_VECTOR(vect, x)	(insert_vector(vect, (vect)->index, x))

#define POP_BACK_VECTOR(vect)		(erase_vector(vect, (vect)->index - 1))
#define POP_FRONT_VECTOR(vect)		(erase_vector(vect, 0))

#define _VECT_SIZE			8

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
void reverse_vector(vector_t*);
int get_index_vector(vector_t*, void const*, int(*)());
void foreach_vector(vector_t*, void(*)());
void append_vector(vector_t*, vector_t const*);
void move_vector(vector_t*, vector_t*);

#endif /* VECTOR_H */
