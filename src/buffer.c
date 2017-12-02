#include <string.h>

#include "buffer.h"
#include "utils.h"

static void make_null_body(buffer_t* buf) {
	for (size_t i = 0; i < buf->alloc; ++i)
		{ buf->body[i] = '\0'; }
}

static buffer_t* make_len_buf(size_t len) {
	buffer_t* buf = NEW(buffer_t, 1);
	if (!buf)
		{ return (NULL); }
	buf->body = NEW(char, len);
	if (!buf->body)
		{ return (NULL); }
	buf->alloc = len;
	make_null_body(buf);
	buf->index = 0;
	return (buf);
}

buffer_t* buffer(void) {
	return (make_len_buf(100));
}

void del_buffer(buffer_t* buf) {
	if (buf) {
		FREE(buf->body);
		FREE(buf);
	}
}

char* body_buffer(buffer_t* buf) {
	if (!buf)
		{ return (NULL); }
	return (buf->body);
}

size_t size_buffer(buffer_t* buf) {
	if (!buf)
		{ return (0); }
	return (buf->index);
}

size_t alloc_size_buffer(buffer_t* buf) {
	if (!buf)
		{ return (0); }
	return (buf->alloc);
}

static void extend_buffer(buffer_t* buf) {
	char* nw_body = (char*)REALLOC(buf->body, buf->alloc);
	if (nw_body) {
		make_null_body(buf);
		buf->body = nw_body;
	}
}

void write_char_buffer(buffer_t* buf, char c) {
	if (buf->index + 1 >= buf->alloc) {
		buf->alloc = round_up(buf->index + 1);
		extend_buffer(buf);
	}
	buf->body[buf->index++] = c;
}

void write_buffer(buffer_t* buf, char const* str) {
	if (!buf || !str)
		{ return; }
	while (*str)
		{ write_char_buffer(buf, *str++); }
	write_char_buffer(buf, '\0');
}

void append_buffer(buffer_t* buf, buffer_t* abuf) {
	if (!buf || !abuf)
		{ return; }
	write_buffer(buf, abuf->body);
}

buffer_t* cpy_buffer(buffer_t* buf) {
	if (!buf)
		{ return (NULL); }
	buffer_t* new_buf = make_len_buf(buf->alloc);
	if (!new_buf)
		{ return (NULL); }
	return (memcpy(new_buf->body, buf->body, sizeof(char) * buf->alloc));
}

void unget_c_buffer(buffer_t* buf, size_t sz) {
	if (!buf)
		{ return; }
	while (buf->index-- && sz--)
		{ buf->body[buf->index] = '\0'; }
}

void reset_buffer(buffer_t* buf) {
	if (!buf)
		{ return; }
	buf->index = 0;
	make_null_body(buf);
}
