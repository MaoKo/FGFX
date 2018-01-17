#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

#define RED_COLOR       ("\e[31m")
#define ORANGE_COLOR    ("\e[33m")
#define NO_COLOR        ("\e[0m")

void errorf(size_t, char const*, ...);
void warnf(size_t, char const*, ...);

#endif /* ERROR_H */
