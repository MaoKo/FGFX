#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

size_t
round_up(size_t size) {
	size_t close2 = 1;
	while (close2 <= size)
		{ close2 <<= 1; }
	return (close2);
}

char*
strjoin(char const* str1, char const* str2) {
	size_t const s1 = strlen(str1);
	size_t const s2 = strlen(str2);
	char* frame = NEW(char, s1 + s2 + 1);
	if (!frame)
		{ return (NULL); }
	memcpy(frame, str1, sizeof(char) * s1);
	memcpy(frame + s1, str2, sizeof(char) * s2);
	frame[s1 + s2] = EOS;
	return (frame);
}

char const*
get_filename(char const* path) {
	char const* last = path;
	while (*path) {
		if (*path++ == '/')
			{ last = path; }
	}
	return (last);
}

trans_list_t*
new_trans_list(unsigned int input, unsigned int state) {
	trans_list_t* list = NEW(trans_list_t, 1);
	if (!list)
		{ return (NULL); }
	list->input = input;
	list->state = state;
	list->next = NULL;
	return (list);
}

void
del_trans_list(trans_list_t* list) {
	while (list) {
		trans_list_t* next = list->next;
		FREE(list);
		list = next;
	}
}

bool
cmp_trans_list(trans_list_t const* t1, trans_list_t const* t2) {
	while (t1 && t2) {
		if ((t1->input != t2->input) || (t1->state != t2->state))
			{ return (false); }
		t1 = t1->next;
		t2 = t2->next;
	}
	return (!t1 && !t2);
}

trans_list_t const*
contiguous_range(trans_list_t const* t) {
	while (t && t->next) {
		if (((t->input - 1) != (t->next->input))
				|| (t->state != t->next->state))
			{ break;  }
		t = t->next;
	}
	return (t);	
}

size_t
hash_str(char const* str) {
	size_t posc = EOS;
	while (*str)
		{ posc += *(str++); }
	return (posc);
}

bool
file_exist(char const* filename) {
	return (open(filename, O_WRONLY | O_EXCL) != -1);
}

void*
_pmalloc(size_t size) {
	if (!size)
		{ size = 1; }
	void* chunk = malloc(size);
#ifndef NDEBUG
	assert(chunk);
#else
	if (!chunk) {
		fprintf(stderr, "No enough memory for this process.\n");
		errno = ENOMEM;
	}
#endif
	return (chunk);
}

void*
_prealloc(void* ptr, size_t size) {
	if (!size) {
		FREE(ptr);
		return (NULL);
	}
	void* new_chunk = realloc(ptr, size);
#ifndef NDEBUG
	assert(new_chunk);
#else
	if (!chunck) {
		fprintf(stderr, "The reallocation has failed.\n");
		errno = ENOMEM;
	}
#endif
	return (new_chunk);
}
