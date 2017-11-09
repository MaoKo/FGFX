#include "utils.h"

size_t round_up(size_t size) {
	size_t close2 = 1;
	while (close2 <= size)
		{ close2 *= 2; }
	return (close2);
}

void* __pmalloc(size_t size) {
	if (!size)
		{ size = 1; }
	void* chunk = malloc(size);
#ifndef NDEBUG
	assert(chunk);
#else
	if (!chunk) {
		WRITE_ERR("No enough memory for this process.\n");
		errno = ENOMEM;
	}
#endif
	return (chunk);
}

void* __prealloc(void* ptr, size_t size) {
	if (!size) {
		FREE(ptr);
		return (NULL);
	}
	void* new_chunk = realloc(ptr, size);
#ifndef NDEBUG
	assert(new_chunk);
#else
	if (!chunck) {
		WRITE_ERR("The reallocation has failed.\n");
		errno = ENOMEM;
	}
#endif
	return (new_chunk);
}
