#include "utils.h"

size_t round_up(size_t size) {
	size_t close2 = 1;
	while (close2 <= size)
		{ close2 *= 2; }
	return (close2);
}

char* strjoin(char const* str1, char const* str2) {
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

int write_str_fd(int filde, char const* str) {
	return (write(filde, str, strlen(str)));
}

int write_str(char const* str) {
	return (write(fileno(stdout), str, strlen(str)));
}

int write_str_err(char const* str) {
	return (write(fileno(stderr), str, strlen(str)));
}

void* _pmalloc(size_t size) {
	if (!size)
		{ size = 1; }
	void* chunk = malloc(size);
#ifndef NDEBUG
	assert(chunk);
#else
	if (!chunk) {
		write_str_err("No enough memory for this process.\n");
		errno = ENOMEM;
	}
#endif
	return (chunk);
}

void* _prealloc(void* ptr, size_t size) {
	if (!size) {
		FREE(ptr);
		return (NULL);
	}
	void* new_chunk = realloc(ptr, size);
#ifndef NDEBUG
	assert(new_chunk);
#else
	if (!chunck) {
		write_str_err("The reallocation has failed.\n");
		errno = ENOMEM;
	}
#endif
	return (new_chunk);
}
