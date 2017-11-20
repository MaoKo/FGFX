#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

//Allocation Macro
#define NEW(type, size)		(type*)_pmalloc(sizeof(type)*size)
#define REALLOC(ptr, size)	_prealloc(ptr, size)
#define FREE(ptr)		free((void*)ptr)

#define BUFF_SIZE		16
#define VECT_SIZE		8

#define EOS			'\0'
#define EMPTY_SET		NULL

#define MIN_ASCII		0
#define MAX_ASCII		(UCHAR_MAX + 1)

#define EPSILON			(MAX_ASCII + 1)

#define ERROR			-1

size_t round_up(size_t);
char* strjoin(char const*, char const*);
bool file_exist(char const*);
int write_str(char const*);
int write_str_err(char const*);
int write_str_fd(int, char const*);
void* _pmalloc(size_t);
void* _prealloc(void*, size_t);

#endif /* UTILS_H */
