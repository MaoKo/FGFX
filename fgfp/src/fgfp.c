#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "cfg_production.h"
#include "ll.h"
#include "lr.h"

#include "gen_lr.h"
#include "gen_ll.h"
#include "gen_cfg.h"

#include "gen.h"
#include "error.h"
#include "bitset.h"
#include "utils.h"

static int
gen_fgfp_file(cfg_t const* cfg, char const* base_file) {
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);

	if (filde == -1)
		{ return (ERROR); }

	gen_require_macro(filde, base_file);

	gen_location_token(filde, cfg);
	gen_non_terminal_enum(filde, cfg);	

	gen_production_enum(filde, cfg);

	gen_rhs_table(filde, cfg, header);	
	gen_lhs_table(filde, cfg, header);	

	gen_ll_useful_macro(filde, cfg);

	bool ll1 = false;
	if (is_ll1(cfg)) {
		ll1 = true;
		vector_t* ll1_table = build_ll1_table(cfg);

		gen_ll_table(filde, cfg, ll1_table, header);
		foreach_vector(ll1_table, &del_trans_list);

		del_vector(ll1_table);
	}

	if (ll1)
		{ PRINT_IF_0(filde); }

	vector_t* lr1_states = build_lalr1_states(cfg);
	compute_reduce_op(cfg, lr1_states);

	gen_action_enum(filde);
	gen_lr_useful_macro(filde);

	gen_action_table(filde, cfg, lr1_states, header);
	gen_goto_table(filde, cfg, lr1_states, header);

//	print_debug_report(cfg, lr1_states);

	foreach_vector(lr1_states, &del_lr1_state);
	del_vector(lr1_states);

	del_lalr_record();

	if (ll1) {
		PRINT_END_IF(filde);
		gen_synchronizing_token(filde, cfg, header);
	}

	gen_endif(filde, base_file);

	FREE(header);
	if (close(filde) == -1)
		{ return (ERROR); }

	return (DONE);	
}

int main(int argc, char* argv[]) {
	if (argc <= 1)
		{ return (EXIT_FAILURE); }

	int filde = open(argv[1], O_RDONLY);
	if (filde == -1) {
		errorf(0, "Can't open %s.", argv[1]);
		return (EXIT_FAILURE);
	}

	cfg_t* cfg = parse_cfg(filde);
	if (!cfg)
		{ return (EXIT_FAILURE); }

	augment_grammar(cfg);
	if (cfg_sanity_check(cfg) == ERROR) {
		del_cfg(cfg);
		return (EXIT_FAILURE);
	}

//	print_production(cfg);
	int exit_st = gen_fgfp_file(cfg, get_filename(argv[1]));

#if 0
	print_production(cfg);
	print_non_terminal(cfg);
	print_nullable(cfg);
	print_first_set(cfg);
	print_follow_set(cfg);
#endif	

	del_cfg(cfg);

	if (close(filde) == -1) {
		errorf(0, "Can't close %s.", argv[1]);
		return (EXIT_FAILURE);
	}

	return (exit_st);
}
