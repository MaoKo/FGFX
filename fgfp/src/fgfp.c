#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cfg.h"
#include "bitset.h"
#include "cfg_production.h"
#include "ll.h"
#include "lr.h"
#include "display_lr.h"
#include "display_ll.h"
#include "lalr.h"
#include "utils.h"

int main(int argc, char* argv[]) {
	if (argc <= 1)
		{ exit(1); }
	int filde = open(argv[1], O_RDONLY);
	cfg_t* cfg = parse_cfg(filde);

	if (!cfg)
		{ exit(EXIT_FAILURE); }
	augment_grammar(cfg);

	if (cfg_sanity_check(cfg) == ERROR) {
		del_cfg(cfg);
		exit(EXIT_FAILURE);
	}

#if 0
	vector_t* lr1_states = gen_lalr1_states(cfg);

	compute_reduce_op(cfg, lr1_states);
	print_debug_report(cfg, lr1_states);

	del_record();

	foreach_vector(lr1_states, &del_lr1_state);
	del_vector(lr1_states);
#endif

	display_parsing_table(cfg, get_filename(argv[1]));

#if 0
	print_non_terminal(cfg);
	print_production(cfg);
	print_nullable(cfg);
	print_first_set(cfg);
	print_follow_set(cfg);
#endif	

	del_cfg(cfg);
	return (EXIT_SUCCESS);
}
