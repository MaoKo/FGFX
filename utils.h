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
//DEBUG
#include <stdio.h>

//Allocation Macro
#define NEW(type, size)		(type*)__pmalloc(sizeof(type)*size)
#define REALLOC(ptr, size)	__prealloc(ptr, size)
#define FREE(ptr)		free((void*)ptr)

//Output Macro
#define WRITE(filde, str)	write(filde, str, sizeof(str))
#define WRITE_ERR(str)		WRITE(STDERR_FILENO, str)

#define VECT_SIZE		8
#define EOS			'\0'
#define EMPTY_SET		NULL
#define EPSILON			(UCHAR_MAX + 1)
#define MIN_ASCII		0
#define MAX_ASCII		SCHAR_MAX

size_t round_up(size_t);
void* __pmalloc(size_t);
void* __prealloc(void*, size_t);

#endif /* UTILS_H */
