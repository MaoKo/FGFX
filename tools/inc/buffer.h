#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#define BODY_BUFFER(buf)        ((buf)->body)
#define SIZE_BUFFER(buf)        ((buf)->index)
#define CHAR_AT(buf, index)     ((buf)->body[index])
#define LAST_CHAR(buf)          ((buf)->body[(buf)->index - 1])
#define ALLOC_SIZE_BUFFER(buf)  ((buf)->alloc)
#define CMP_BUFF_STR(buf, str)  (!strcmp((buf)->body, str))
#define CMP_BUFFER(b1, b2)      (CMP_BUFF_STR(b1, (b2)->body))

#define _BUFF_SIZE              (16)

typedef struct {
    char* body;
    size_t alloc;
    size_t index;
} buffer_t;

buffer_t* new_buffer(void);
buffer_t* dup_buffer(buffer_t const*);
void del_buffer(buffer_t*);
int write_char_buffer(buffer_t*, char);
int write_buffer(buffer_t*, char const*);
int append_buffer(buffer_t*, buffer_t const*);
void unget_char_front_buffer(buffer_t*, size_t);
void unget_char_back_buffer(buffer_t*, size_t);
void reset_buffer(buffer_t*);
int hash_buffer(buffer_t const*);
void move_back_buffer(buffer_t*, buffer_t*, size_t);
void move_front_buffer(buffer_t*, buffer_t*, size_t);

#endif /* BUFFER_H */
