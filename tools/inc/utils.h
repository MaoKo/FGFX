#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

//Allocation Macro
#define NEW(type, size)		(type*)_pmalloc(sizeof(type) * size)
#define REALLOC(ptr, size)	_prealloc(ptr, size)
#define FREE(ptr)		free((void*)ptr)

//Misc
#define MIN(x, y)		((x < y) ? x : y)
#define MAX(x, y)		((x > y) ? x : y)

#define EOS			'\0'
#define ERROR			-1
#define DONE			0

#define NULL_TRANS_LST		(NULL)

typedef struct trans_list_t {
	unsigned int input;
	unsigned int state;
	struct trans_list_t* next;
} trans_list_t;

size_t round_up(size_t);
char* strjoin(char const*, char const*);
char const* get_filename(char const*);
size_t char_in_str(char const*, char);
trans_list_t* new_trans_list(unsigned int, unsigned int);
void del_trans_list(trans_list_t*);
bool cmp_trans_list(trans_list_t const*, trans_list_t const*);
trans_list_t const* contiguous_range(trans_list_t const*);
size_t hash_str(char const*);
bool file_exist(char const*);
void* _pmalloc(size_t);
void* _prealloc(void*, size_t);

#endif /* UTILS_H */
