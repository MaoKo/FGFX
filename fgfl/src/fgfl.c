#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "token_def.h"
#include "preprocess_regex.h"
#include "nfa.h"
#include "dfa.h"
#include "error.h"
#include "utils.h"

int
main(int argc, char const* argv[]) {
	if (argc <= 1)
		{ return (EXIT_FAILURE); }

	int filde = open(argv[1], O_RDONLY);
	if (filde == -1) {
		errorf(0, "Can't open %s.", argv[1]);
		return (EXIT_FAILURE);
	}

	token_spec_t* spec = parse_token_def(filde);
	if (!spec)
		{ exit(EXIT_FAILURE); }

	vector_t* stack_order;
	if (!(stack_order = topological_sort(spec))) {
		del_token_spec(spec);
		return (EXIT_FAILURE);
	}

	for (size_t i = 0; i < SIZE_VECTOR(stack_order); ++i)
		{ printf("%p\n", AT_VECTOR(stack_order, i)); }

	del_vector(stack_order);

//	if (!EMPTY_VECTOR(spec->entry_lst)) {
//		nfa_gen(spec);
//		dfa_gen(spec, get_filename(path));
//	}

	del_token_spec(spec);

	if (close(filde) == -1) {
		errorf(0, "Can't close %s.", argv[1]);
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
