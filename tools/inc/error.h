#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

void errorf(size_t, char const*, ...);
void warnf(size_t, char const*, ...);

#endif /* ERROR_H */
