#include <string.h>

#include "buffer.h"
#include "utils.h"

static buffer_t*
make_len_buf(size_t len) {
	buffer_t* buf = NEW(buffer_t, 1);
	if (!buf)
		{ return (NULL); }
	buf->body = NEW(char, len);
	if (!buf->body)
		{ return (NULL); }
	buf->alloc = len;
	buf->index = 0;
	memset(buf->body, 0, buf->alloc);
	return (buf);
}

buffer_t*
new_buffer(void) {
	return (make_len_buf(BUFF_SIZE));
}

buffer_t*
dup_buffer(buffer_t const* buf) {
	if (!buf)
		{ return (NULL); }
	buffer_t* new_buf = make_len_buf(buf->alloc);
	if (!new_buf)
		{ return (NULL); }
	memcpy(new_buf->body, buf->body, buf->alloc);
	return (new_buf);
}

void
del_buffer(buffer_t* buf) {
	if (buf)
		{ FREE(buf->body); }
	FREE(buf);
}

static int
extend_buffer(buffer_t* buf) {
	if (!buf)
		{ return (-1); }
	if (buf->index + 1 >= buf->alloc) {
		size_t new_sz = round_up(buf->index + 1);
		char* nw_body = (char*)REALLOC(buf->body, new_sz);
		if (nw_body) {
			buf->body = nw_body;
			buf->alloc = new_sz;
			memset(buf->body + buf->index, 0, buf->alloc - buf->index);
		}
		else
			{ return (-1); }
	}
	return (0);
}

int
write_char_buffer(buffer_t* buf, char c) {
	if (extend_buffer(buf))
		{ return (-1); }
	return (buf->body[buf->index++] = c);
}

int
write_buffer(buffer_t* buf, char const* str) {
	if (!buf || !str)
		{ return (-1); }
	while (*str) {
		if (write_char_buffer(buf, *str++))
			{ return (-1); }
	}
	write_char_buffer(buf, '\0');
	return (strlen(str));
}

int
append_buffer(buffer_t* buf, buffer_t const* src) {
	if (!buf || !src)
		{ return (-1); }
	return (write_buffer(buf, src->body));
}

void
unget_c_buffer(buffer_t* buf, size_t sz) {
	if (!buf)
		{ return; }
	while (buf->index-- && sz--)
		{ buf->body[buf->index] = '\0'; }
}

void
reset_buffer(buffer_t* buf) {
	if (!buf)
		{ return; }
	memset(buf->body, 0, buf->alloc);
	buf->index = 0;
}

int
hash_buffer(buffer_t const* buf) {
	if (!buf)
		{ return (0); }
	int sum_char = 0;
	for (size_t i = 0; i < buf->index; ++i)
		{ sum_char += buf->body[i]; }
	return (sum_char);
}
