#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define BUFF_SIZE		16

typedef struct {
	char* body;
	size_t alloc;
	size_t index;
} buffer_t;

buffer_t* buffer(void);
void del_buffer(buffer_t*);
char* body_buffer(buffer_t*);
size_t size_buffer(buffer_t*);
size_t alloc_size_buffer(buffer_t*);
void write_char_buffer(buffer_t*, char);
void write_buffer(buffer_t*, char const*);
void append_buffer(buffer_t*, buffer_t*);
buffer_t* cpy_buffer(buffer_t*);
void unget_c_buffer(buffer_t*, size_t);
void reset_buffer(buffer_t*);

#endif /* BUFFER_H */
