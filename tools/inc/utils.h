#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>

//Allocation Macro
#define NEW(type, size)		(type*)_pmalloc(sizeof(type) * size)
#define REALLOC(ptr, size)	_prealloc(ptr, size)
#define FREE(ptr)			free((void*)ptr)

//Misc
#define MIN(x, y)			((x < y) ? x : y)
#define MAX(x, y)			((x > y) ? x : y)

#define EOS					'\0'
#define ERROR				-1
#define DONE				0

#define NULL_TRANS_LST		(NULL)

typedef struct trans_list_t {
	struct trans_list_t* next;
	unsigned int input;
	unsigned int state;
} trans_list_t;

typedef struct gen_list_t {
	struct gen_list_t* next;
} gen_list_t;

size_t round_up(size_t);
char* strjoin(char const*, char const*);
char const* get_filename(char const*);
int is_tab_or_space(int);
size_t count_front(char const*, int(*)(int));
size_t count_back(char const*, int(*)(int));
size_t char_in_str(char const*, char);
trans_list_t* new_trans_list(unsigned int, unsigned int);
size_t size_gen_list(gen_list_t const*);
void redirect_trans_list(trans_list_t*, unsigned int, unsigned int);
void append_trans_list(trans_list_t*, trans_list_t*);
void del_trans_list(trans_list_t*);
bool cmp_input_trans_list(trans_list_t const*, unsigned int);
bool cmp_trans_list(trans_list_t const*, trans_list_t const*);
trans_list_t const* contiguous_range(trans_list_t const*);
size_t hash_str(char const*);
int open_new_file(char const*);
void* _pmalloc(size_t);
void* _prealloc(void*, size_t);

#endif /* UTILS_H */
