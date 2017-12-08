#include <stdlib.h>

#include "token_def.h"
#include "nfa.h"
#include "dfa.h"
#include "utils.h"

int
main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ exit(EXIT_FAILURE); }

#if 0
	bitset_t* me = new_bitset();
	ADD_BITSET(me, 512);
	print_bitset(me);
	printf("%d\n", is_empty_bitset(me));

	exit(1);
#endif
	char const* path = argv[1];
	token_spec_t* spec = NULL;

	Reggen(argv[1], &spec);
	NFAgen(spec);
	DFAgen(spec, get_filename(path));

	return (0);
}
