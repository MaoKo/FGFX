#include "tokendef.h"
#include "dfa.h"

/*
static char const* get_filename(char const* path) {
	char const* last = path;
	while (*path) {
		if (*path++ == '/')
			{ last = path; }
	}
	return (last);
}
*/

int main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(EXIT_FAILURE); }

	char const* path = argv[1];
	token_spec_t* spec = NULL;

	(void)path;

	Reggen(argv[1], &spec);
	NFAgen(spec);
	DFAgen(spec, "a");

	return (0);
}
