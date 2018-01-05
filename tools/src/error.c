#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "error.h"

static void
print_error(size_t lineno, char const* label, char const* color,
								char const* format, va_list args) {
	// Color Output
	int tty = isatty(STDERR_FILENO);
	if (tty)
		{ fprintf(stderr, color); }
	fprintf(stderr, "(%s", label);
	if (lineno)
		{ fprintf(stderr, ":%zu", lineno); }
	fprintf(stderr, ") ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	if (tty)
		{ fprintf(stderr, NO_COLOR); }
}

void
errorf(size_t lineno, char const* format, ...) {
	va_list args;
	va_start(args, format);
	print_error(lineno, "ERROR", RED_COLOR, format, args);
	va_end(args);
}

void
warnf(size_t lineno, char const* format, ...) {
	va_list args;
	va_start(args, format);
	print_error(lineno, "WARNING", ORANGE_COLOR, format, args);
	va_end(args);
}

