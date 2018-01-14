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
	return (make_len_buf(_BUFF_SIZE));
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
		{ return (ERROR); }
	if (SIZE_BUFFER(buf) + 1 >= buf->alloc) {
		size_t new_sz = round_up(SIZE_BUFFER(buf) + 1);
		char* nw_body = (char*)REALLOC(buf->body, new_sz);
		if (nw_body) {
			buf->body = nw_body;
			buf->alloc = new_sz;
			memset(buf->body + SIZE_BUFFER(buf), 0,
                                        buf->alloc - SIZE_BUFFER(buf));
		}
		else
			{ return (ERROR); }
	}
	return (DONE);
}

int
write_char_buffer(buffer_t* buf, char c) {
	if (extend_buffer(buf) == ERROR)
		{ return (ERROR); }
	buf->body[SIZE_BUFFER(buf)] = c;
	if (c)
		{ ++(buf->index); }
	return (c);
}

int
write_buffer(buffer_t* buf, char const* str) {
	if (!buf || !str)
		{ return (ERROR); }
	while (*str) {
		if (write_char_buffer(buf, *str++) == ERROR)
			{ return (ERROR); }
	}
	write_char_buffer(buf, '\0');
	return (strlen(str));
}

int
append_buffer(buffer_t* buf, buffer_t const* src) {
	if (!buf || !src)
		{ return (ERROR); }
	return (write_buffer(buf, src->body));
}

void
unget_char_front_buffer(buffer_t* buf, size_t sz) {
    if (!buf)
        { return; }
    else if (SIZE_BUFFER(buf) <= sz)
        { reset_buffer(buf); }
    else {
        buf->index -= sz;
        memmove(buf->body, buf->body + sz, SIZE_BUFFER(buf));
        memset(buf->body + SIZE_BUFFER(buf), 0, sz);
    }
}

void
unget_char_back_buffer(buffer_t* buf, size_t sz) {
	if (!buf)
		{ return; }

    if (sz > SIZE_BUFFER(buf))
		{ sz = SIZE_BUFFER(buf); }
	while (sz--)
		{ buf->body[--(buf->index)] = '\0'; }
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
	for (size_t i = 0; i < SIZE_BUFFER(buf); ++i)
		{ sum_char += buf->body[i]; }
	return (sum_char);
}
