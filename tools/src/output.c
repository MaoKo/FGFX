#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "output.h"

uint8_t
min_size_type(size_t size) {
	if (size <= UCHAR_MAX) { return (8); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (16); }
	return (32);
}

void
output_include_macro(int filde, char const* header) {
	write(filde, "_", 1);
	while (*header) {
		if (*header == '.')
			{ break; }
		char c;
		if (isalnum(*header))
			{ c = toupper(*header); }
		else
			{ c = '_'; }	
		write(filde, &c, 1);
		++header;
	}
	write(filde, "_H", 2);
}

void
output_require_macro(int filde, char const* header) {
	dprintf(filde, "#ifndef ");
	output_include_macro(filde, header);
	dprintf(filde, "\n");
	dprintf(filde, "#define ");
	output_include_macro(filde, header);
	dprintf(filde, "\n\n");
}

void
output_endif(int filde, char const* header) {
	dprintf(filde, "#endif /* ");
	output_include_macro(filde, header);
	dprintf(filde, " */");
}

void
output_verbatim_file(int filde, char const* header) {
	if (isdigit(*header))
		{ write(filde, "_", 1); }
	while (*header && (isalnum(*header) || (*header == '_')))
		{ write(filde, header++, 1); }
}

