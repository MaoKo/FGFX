#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

#include "fgfl.h"

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

size_t round_up(size_t);
char* strjoin(char const*, char const*);
void del_trans_list(trans_list_t*);
bool cmp_trans_list(trans_list_t const*, trans_list_t const*);
trans_list_t const* contiguous_range(trans_list_t const*);
size_t hash_str(char const*);
int cmp_entry_str(token_entry_t const*, char const*);
bool file_exist(char const*);
void* _pmalloc(size_t);
void* _prealloc(void*, size_t);

#endif /* UTILS_H */
