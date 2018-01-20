#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lexical_spec.h"
#include "preprocess_regex.h"
#include "nfa.h"
#include "dfa.h"
#include "gen_dfa.h"
#include "gen.h"
#include "error.h"
#include "utils.h"

static void
gen_dfa_final_tables(int filde, lexical_spec_t* spec, nfa_state_t* master,
                                    char const* header, spec_entry_t* state) {
    build_dfa_table(master, spec);
    minimizing_dfa(spec);
#ifdef DFA_OPTIMIZE
    equivalent_state(spec->trans, spec->final);
#endif /* DFA_OPTIMIZE */
    gen_state_table(filde, spec, header, state);
    gen_middle_table(filde, spec, header, state);

    GEN_FINAL_TABLE(filde, spec, header, state);
    GEN_ANCHOR_FINAL_TABLE(filde, spec, header, state);
}

static int 
gen_fgfl_file(lexical_spec_t* spec, char const* base_file) {
    char const* header = strjoin(base_file, ".h");
    int filde = open_new_file(header);

    if (filde == -1)
        { return (ERROR); }

    gen_require_macro(filde, base_file);

    bool active_state = (spec->start_state != -1);
    if (active_state)
        { gen_state_enum(filde, spec); }
    gen_token_enum(filde, spec);

    if (!spec->miss_regex) {
        IFNDEF_ONLY_STATE_TOKEN(filde);
        if (active_state)
            { gen_change_state(filde, header, spec); }

        DFA_MACRO(filde);

        if (!active_state)
            { gen_dfa_final_tables(filde, spec, spec->master, header, NULL); }
        else {
            for (size_t i = 0; i < SIZE_VECTOR(spec->state_vect); ++i) {
                spec_entry_t* crt_state = (spec_entry_t*)
                                        AT_VECTOR(spec->state_vect, i);
                gen_dfa_final_tables(filde, spec,
                                crt_state->state_master, header, crt_state);
            }
        }
        del_nfa_record();

        GEN_LOOK_TABLE(filde, spec, header);
        GEN_SKIP_TABLE(filde, spec, header);

        ENDIF_ONLY_STATE_TOKEN(filde);
    }

    gen_endif(filde, base_file);
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

    lexical_spec_t* spec = parse_lexical_spec(filde);
    if (!spec)
        { exit(EXIT_FAILURE); }
    
    if (spec_sanity_check(spec) == ERROR) {
        del_lexical_spec(spec);
        return (EXIT_FAILURE);
    }

//  print_token_entry(spec);
    if (build_nfa(spec) == ERROR) {
        errorf(0, "Impossible to construct the NFA automaton.");
        return (EXIT_FAILURE);
    }

    int exit_st = gen_fgfl_file(spec, get_filename(argv[1]));
    del_lexical_spec(spec);

    if (close(filde) == -1) {
        errorf(0, "Can't close %s.", argv[1]);
        return (EXIT_FAILURE);
    }

    return (exit_st);
}
