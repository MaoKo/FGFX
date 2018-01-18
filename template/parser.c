#if 0

#include <stdlib.h>

#include "parser.h"
#include "lexer.h"

#include "template.grm.h" // insert the output of FGFP

int
lr_parser(char const* stream) {
    SET_STREAM(stream);

    int value_stack[/* Size of value_stack */];
    int value_top = 0;

    int parse_stack[/* Size of parse_stack */];
    int parse_top = 0;

    parse_stack[parse_top] = LR_START_STATE;
    //Parser Engine
    while (1) {
        int crt_token = peek_token();
        int top_state = parse_stack[parse_top];

        int kind = /* NAME PREFIX */lr_action_table[top_state][crt_token];

        if (kind == ACCEPT)
            { return (EXIT_SUCCESS); }
        else if (kind & _SHIFT) {
            parse_stack[++parse_top] = kind ^ _SHIFT;
            ++value_top;
            advance_token();
        }
        else if (kind & _REDUCE) {
            parse_top -= /* NAME PREFIX */rhs_prod_table[kind ^ _REDUCE];
    
            int uncovered_state = parse_stack[parse_top];
            int lhs_symbol = /* NAME PREFIX */lhs_prod_table[kind ^ _REDUCE];

            parse_stack[++parse_top] = 
                /* NAME PREFIX */lr_goto_table[uncovered_state]
                                                [lhs_symbol] ^ _GOTO;

            //Place to put semantic action
            int nb_symbol = /* NAME PREFIX */rhs_prod_table[kind ^ _REDUCE];
            if (!nb_symbol)
                { ++value_top; }

            int tmp_val = value_stack[value_top - 1];
            switch (kind ^ _REDUCE) {
                /* You can insert here, your semantic action */
                /* case NAME_PROD: ... */
            }
            if (nb_symbol) {
                value_top -= nb_symbol;
                value_stack[value_top++] = tmp_val;
            }
        }
        else {
            int error_accept = ERROR_SLOT;
            while (parse_top >= 0) {
                int top_state = parse_stack[parse_top];
                if ((error_accept = /* NAME PREFIX */lr_action_table[top_state]
                                            [T_ERROR]) & _SHIFT) {
                    parse_stack[++parse_top] = error_accept ^ _SHIFT;
                    break;
                }
                --parse_top;
            }

            if (error_accept != ERROR_SLOT) {
                error_accept ^= _SHIFT;
                while (peek_token() != T_EOF
                        && (/* NAME PREFIX */lr_action_table[error_accept]
                                        [peek_token()] == ERROR_SLOT))
                    { advance_token(); }

                if (peek_token() != T_EOF)
                    { continue; }
            }
            return (EXIT_FAILURE);
        }
    }
}

#endif /* 0 */
