#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "error.h"

static void
print_error(size_t lineno, char const* label,
								char const* format, va_list args) {
	// Color Output
	int tty = isatty(STDERR_FILENO);
	if (tty)
		{ fprintf(stderr, "\e[1;31m"); }
	fprintf(stderr, "(%s", label);
	if (lineno)
		{ fprintf(stderr, ":%zu", lineno); }
	fprintf(stderr, ") ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	if (tty)
		{ fprintf(stderr, "\e[0m"); }
}

void
errorf(size_t lineno, char const* format, ...) {
	va_list args;
	va_start(args, format);
	print_error(lineno, "ERROR", format, args);
	va_end(args);
}

void
warnf(size_t lineno, char const* format, ...) {
	va_list args;
	va_start(args, format);
	print_error(lineno, "WARNING", format, args);
	va_end(args);
}

