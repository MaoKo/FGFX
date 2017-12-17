#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "display.h"

uint8_t
min_size_type(size_t size) {
	if (size <= UCHAR_MAX)
		{ return (BYTE); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (SHORT); }
	return (LONG);
}

void
display_include_macro(int filde, char const* header) {
	write(filde, "_", 1);
	while (*header) {
		char c;
		if (isalnum(*header))
			{ c = toupper(*header); }
		else
			{ c = '_'; }	
		write(filde, &c, 1);
		++header;
	}
	dprintf(filde, HEADER_SUF);
}

void
display_require_macro(int filde, char const* header) {
	dprintf(filde, "#ifndef ");
	display_include_macro(filde, header);
	dprintf(filde, NL);
	dprintf(filde, "#define ");
	display_include_macro(filde, header);
	dprintf(filde, NL NL);
}

void
display_endif(int filde, char const* header) {
	dprintf(filde, "#endif /* ");
	display_include_macro(filde, header);
	dprintf(filde, " */" NL);
}

void
display_verbatim_file(int filde, char const* header) {
	if (isdigit(*header))
		{ write(filde, "_", 1); }
	while (*header && (isalnum(*header) || (*header == '_')))
		{ write(filde, header++, 1); }
}

