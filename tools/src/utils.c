#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

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

int
is_tab_or_space(int c) {
	return (c == '\t' || c == ' ');
}

size_t
count_front(char const* base_str, int (*pred)(int)) {
	if (!base_str || !pred)
		{ return (0); }
	char const* front = base_str;
	while (*base_str && (*pred)(*base_str))
		{ ++base_str; }
	return (base_str - front);
}

size_t
count_back(char const* base_str, int (*pred)(int)) {
	if (!base_str || !pred)
		{ return (0); }
	char const* back = base_str + (strlen(base_str) - 1);
	size_t count = 0;
	while (back >= base_str && (*pred)(*back)) {
		++count;
		--back;
	}
	return (count);
}

size_t
char_in_str(char const* base_str, char c) {
	if (!base_str)
		{ return (0); }
	size_t count = 0;
	while (*base_str) {
		if (*base_str == c)
			{ ++count; }
		++base_str;
	}
	if (!c)
		{ ++count; }
	return (count);
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

size_t
size_gen_list(gen_list_t const* list) {
	size_t crt_size = 0;
	while (list) {
		++crt_size;
		list = list->next;
	}
	return (crt_size);
}

void
append_trans_list(trans_list_t* dst, trans_list_t* src) {
	if (!dst)
		{ return; }
	while (dst->next)
		{ dst = dst->next; }
	dst->next = src;
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
cmp_input_trans_list(trans_list_t const* list, unsigned int input) {
	while (list) {
		if (list->input == input)
			{ return (true); }
		list = list->next;
	}
	return (false);
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

int
open_new_file(char const* path_name) {
	int filde = open(path_name, O_WRONLY | O_CREAT | O_EXCL, 0644);
	if ((filde == -1) && (EEXIST == errno))
		{ filde = open(path_name, O_WRONLY | O_TRUNC); }
	return (filde);
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
