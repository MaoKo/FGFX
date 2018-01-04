#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "token_def.h"
#include "preprocess_regex.h"
#include "nfa.h"
#include "dfa.h"
#include "gen_dfa.h"
#include "gen.h"
#include "error.h"
#include "utils.h"

static int 
gen_fgfl_file(token_spec_t* spec, char const* base_file) {
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);

	if (filde == -1)
		{ return (ERROR); }

	vector_t* trans = NULL;
	vector_t* final = NULL;

	build_dfa_table(spec->master, &trans, &final, spec->entry_lst);

#ifdef OPTIMIZE
	equivalent_state(trans, final);
#endif /* OPTIMIZE */

	gen_require_macro(filde, base_file);
	gen_dfa_typedef(filde, SIZE_VECTOR(trans), SIZE_VECTOR(final));
	gen_token_enum(filde, spec->entry_lst);

	IFNDEF_ONLY_TOKEN(filde);

	gen_dfa_useful_macro(filde);
	gen_state_table(filde, trans, header);
	gen_final_table(filde, final, header);
	gen_skip_table(filde, spec->entry_lst, header);

	ENDIF_ONLY_TOKEN(filde);

	gen_endif(filde, base_file);

	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i)
		{ del_trans_list(AT_VECTOR(trans, i)); }

	del_vector(trans); del_vector(final);
	del_token_spec(spec);

	FREE(header);
	if (close(filde) == -1)
		{ return (ERROR); }

	return (DONE);
}

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

	if (!EMPTY_VECTOR(spec->entry_lst) && compute_regex(spec) != ERROR) {
		nfa_gen(spec);
		gen_fgfl_file(spec, get_filename(argv[1]));
	}

//	del_token_spec(spec);

	if (close(filde) == -1) {
		errorf(0, "Can't close %s.", argv[1]);
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
