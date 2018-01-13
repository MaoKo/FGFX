#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "gen.h"

uint8_t
min_size_type(size_t size, bool unsign) {
	if ((unsign && size <= UCHAR_MAX) || (!unsign && size <= SCHAR_MAX))
		{ return (BYTE); }
	else if ((unsign && size > UCHAR_MAX && size <= USHRT_MAX)
			|| (!unsign && size > SCHAR_MAX && size <= SHRT_MAX))
		{ return (SHORT); }
	return (LONG);
}

void
gen_include_macro(int filde, char const* header) {
	write(filde, SEP, 1);
	while (*header) {
		char c;
		if (isalnum(*header))
			{ c = toupper(*header); }
		else
			{ c = '_'; }	
		write(filde, &c, 1);
		++header;
	}
	dprintf(filde, SEP HEADER_SUF SEP);
}

void
gen_require_macro(int filde, char const* header) {
	dprintf(filde, "#ifndef ");
	gen_include_macro(filde, header);
	dprintf(filde, NL "#define ");
	gen_include_macro(filde, header);
	dprintf(filde, NL NL);
	dprintf(filde, INCLUDE_SYS(stdint.h) NL);
	dprintf(filde, INCLUDE_SYS(stdbool.h) NL NL);
}

void
gen_endif(int filde, char const* header) {
	dprintf(filde, "#endif /* ");
	gen_include_macro(filde, header);
	dprintf(filde, " */" NL);
}

void
gen_verbatim_file(int filde, char const* header) {
	if (isdigit(*header))
		{ write(filde, "_", 1); }
	while (*header && (isalnum(*header) || (*header == '_')))
		{ write(filde, header++, 1); }
}

