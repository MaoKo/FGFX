#include <stdlib.h>

#include "token_def.h"
#include "nfa.h"
#include "dfa.h"
#include "utils.h"

int
main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(EXIT_FAILURE); }

	char const* path = argv[1];
	token_spec_t* spec = NULL;

	regex_gen(argv[1], &spec);
	nfa_gen(spec);
	dfa_gen(spec, get_filename(path));

	return (EXIT_SUCCESS);
}
