#include <stdio.h>

#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "lexer.h"
#include "fgfx.lex.h"
#include "error.h"
#include "utils.h"
#include "buffer.h"

lexer_t*
new_lexer(int filde) {
    lexer_t* lex = NEW(lexer_t, 1);

    if (!lex)
        { return (NULL); }
    memset(lex, 0, sizeof(*lex));

    lex->filde = filde;

    lex->push_back = new_buffer();
    lex->last_lexeme = new_buffer();

    lex->last_token = NO_TOKEN;
    lex->lineno = START_LINE;
    
    lex->stack_state = new_vector();
    lex->crt_state = INIT_STATE;

    return (lex);
}

void
del_lexer(lexer_t* lex) {
    if (lex) {
        del_buffer(lex->push_back);
        del_buffer(lex->last_lexeme);
        del_vector(lex->stack_state);
    }
    FREE(lex);
}

static size_t
is_final_state(int state, uint8_t (*final_table)[2]) {
    size_t i = 0;
    while (*final_table[i]) {
        if (*final_table[i] == state)
            { return (final_table[i][1]); }
        ++i;
    }
    return (T_ERROR);
}

static void
change_lexer_state(lexer_t* lex,
                uint8_t (**state_table)[256], uint8_t (**final_table)[2]) {
    switch (lex->crt_state) {
        case S_GLOBAL:
            (*state_table) = fgfx_GLOBAL_state_table;
            (*final_table) = fgfx_GLOBAL_final_table;
            break;

        case S_BEG_REGEX:
            (*state_table) = fgfx_BEG_REGEX_state_table;
            (*final_table) = fgfx_BEG_REGEX_final_table;
            break;

        case S_BODY_REGEX:
            (*state_table) = fgfx_BODY_REGEX_state_table;
            (*final_table) = fgfx_BODY_REGEX_final_table;
            break;

        case S_STRING:
            (*state_table) = fgfx_STRING_state_table;
            (*final_table) = fgfx_STRING_final_table;
            break;

        case S_FINITE_SEQ:
            (*state_table) = fgfx_FINITE_SEQ_state_table;
            (*final_table) = fgfx_FINITE_SEQ_final_table;
            break;

        case S_BEG_CCL:
            (*state_table) = fgfx_BEG_CCL_state_table;
            (*final_table) = fgfx_BEG_CCL_final_table;
            break;

        case S_BODY_CCL:
            (*state_table) = fgfx_BODY_CCL_state_table;
            (*final_table) = fgfx_BODY_CCL_final_table;
            break;

        case S_NESTED_COM:
            (*state_table) = fgfx_NESTED_COM_state_table;
            (*final_table) = fgfx_NESTED_COM_final_table;
            break;

        case S_REG_COM:
            (*state_table) = fgfx_REG_COM_state_table;
            (*final_table) = fgfx_REG_COM_final_table;
            break;

        case S_REG_PARAMS:
            (*state_table) = fgfx_REG_PARAMS_state_table;
            (*final_table) = fgfx_REG_PARAMS_final_table;
            break;

        case S_REG_NO_PARAMS:
            (*state_table) = fgfx_REG_NO_PARAMS_state_table;
            (*final_table) = fgfx_REG_NO_PARAMS_final_table;
            break;
    }
}

static inline bool
must_look_ahead(int crt_state, int state) {
    if (crt_state == S_BODY_CCL && fgfx_BODY_CCL_ahead_table[state])
        { return (true); }
    else if (crt_state == S_BEG_REGEX && fgfx_BEG_REGEX_ahead_table[state])
        { return (true); }
    else if (crt_state == S_BODY_REGEX && fgfx_BODY_REGEX_ahead_table[state])
        { return (true); }
    return (false);
}

static inline void
change_state(lexer_t* lex, int last_match, bool* need_recompute) {
    int action = fgfx_action_table[last_match][lex->crt_state];
    if (action) {
        if (action & _BEGIN)
            { lex->crt_state = action ^ _BEGIN; }
        else if (action & _PUSH) {
            PUSH_BACK_VECTOR(lex->stack_state, (void*)lex->crt_state);
            lex->crt_state = action ^ _PUSH;
        }
        else {
            lex->crt_state = (size_t)BACK_VECTOR(lex->stack_state);
            POP_BACK_VECTOR(lex->stack_state);
        }
        (*need_recompute) = true; 
    }
}

static int
get_next_token(lexer_t* lex) {
    static uint8_t (*state_table)[256] = NULL;
    static uint8_t (*final_table)[2] = NULL;

    static bool need_recompute = true;

    int state = START_STATE;
    int last_match = T_ERROR;

    int rd = 0;

    if (need_recompute) {
        change_lexer_state(lex, &state_table, &final_table);
        need_recompute = false;
    }

    size_t read_char = 0;
    size_t pos_last_match = 0;

    size_t i;
    for (i = 0; i < SIZE_BUFFER(lex->push_back); ++i) {
        int char_at = CHAR_AT(lex->push_back, i);
        ++read_char;

        state = state_table[state][char_at];

        if (state == DEAD_STATE)
            { break; }
        else { 
            if (char_at == '\n')
                { ++(lex->lineno); }
            if (is_final_state(state, final_table) != T_ERROR) {
                last_match = is_final_state(state, final_table);
                pos_last_match = read_char;
            }
            write_char_buffer(lex->last_lexeme, char_at);
        }
    }

    unget_char_front_buffer(lex->push_back, i);

    int first_read = true;
    bool unget_input = false;

    while (state != DEAD_STATE) {
        int exit_st = read(lex->filde, &rd, 1);
        ++read_char;

        if (!exit_st) {
            if (first_read) {
                if (EMPTY_VECTOR(lex->stack_state))
                    { return (T_EOF); }
                errorf(0, "The input is terminated but many "
                                            "states still live on the stack.");
                return (T_ERROR);
            }
            break;
        }

        // Change state
        state = state_table[state][rd];
        if (is_final_state(state, final_table) != T_ERROR) {
            last_match = is_final_state(state, final_table);
            pos_last_match = read_char;
        }

        if (must_look_ahead(lex->crt_state, state)) {
            if (unget_input) {
                append_buffer(lex->last_lexeme, lex->push_back);
                reset_buffer(lex->push_back);
            }
            unget_input = true;
        }

        // Write on buffer
        if (unget_input)
            { write_char_buffer(lex->push_back, rd); }
        else {
            if ((rd == '\n') && (state != DEAD_STATE))
                { ++(lex->lineno); }
            write_char_buffer(lex->last_lexeme, rd);
        }

        first_read = false;
    }

    if (last_match == T_ERROR) {
        char const* error_str = C_LEXEME(lex);
        if (!strcmp(error_str, "\n"))
            { error_str = "\\n"; }

        errorf(CURRENT_LINE(lex),
                        "Lexical error on input '%s'.", error_str);
    }
    else if (!first_read) {
        if (unget_input) {
            if (!check_present_table(fgfx_look_table, last_match)) {
                unget_char_back_buffer(lex->push_back, 1);

                append_buffer(lex->last_lexeme, lex->push_back);
                reset_buffer(lex->push_back);

                write_char_buffer(lex->push_back, rd);
            }
        }
        else {
            move_back_buffer(lex->push_back,
                            lex->last_lexeme, read_char - pos_last_match);
        }
    }

    change_state(lex, last_match, &need_recompute);
    return (last_match);
}

bool
check_present_table(int8_t* table, int token) {
    for (size_t i = 0; table[i] != -1; ++i) {
        if (token == table[i])
            { return (true); }
    }
    return (false);
}

long
advance_token(lexer_t* lex) {
    if (!lex)
        { return (ERROR); }
    long found_token;
    if (lex->last_token != -1)
        { found_token = lex->last_token; }
    else {
        reset_buffer(lex->last_lexeme);
        while (1) {
            found_token = get_next_token(lex);
            if (!check_present_table(fgfx_skip_table, found_token))
                { break; }
            reset_buffer(lex->last_lexeme);
        }
    }

    if (found_token == T_DIRECTIVE) {
        static void* directive_tab[][2] = {
            { (void*)T_SKIP,        "$SKIP" },
            { (void*)T_TOKEN,       "$TOKEN" },
            { (void*)T_KEYWORD,     "$KEYWORD" },
            { (void*)T_R_IGCASE,      "$IGCASE" },
            { (void*)T_STATE,       "$STATE" },
            { (void*)T_BEGIN,       "$BEGIN" },
            { (void*)T_PUSH,        "$PUSH" },
            { (void*)T_POP,         "$POP" },
            { (void*)T_FRAGMENT,    "$FRAGMENT" },
            { (void*)T_INITIAL,     "$INITIAL" },
            { (void*)T_EXTERN,      "$EXTERN" },
            { (void*)T_ALIAS,       "$ALIAS" },
            { (void*)T_PRODUCTION,  "$PRODUCTION" },
            { (void*)T_PRECEDENCE,  "$PRECEDENCE" },
            { (void*)T_MIMIC,       "$MIMIC" },
            { (void*)T_EMPTY,       "$EMPTY" },
            { (void*)T_START,       "$START" },
            { (void*)T_LEFT,        "$LEFT" },
            { (void*)T_RIGHT,       "$RIGHT" },
            { (void*)T_NONASSOC,    "$NONASSOC" },
            { (void*)T_STAY,        "$STAY" },
        };
        for (size_t i = 0; i < *(&directive_tab + 1) - directive_tab; ++i) {
            if (!strcmp(C_LEXEME(lex), directive_tab[i][1]))
                { return ((long)*directive_tab[i]); }
        }
        errorf(CURRENT_LINE(lex), "Bad directive %s.", C_LEXEME(lex));
        return (T_ERROR);
    }
    else if (found_token == T_CCE) {
        static void* cce_tab[][2] = {
            { (void*)T_CCE_ALNUM,   "[:alnum:]"  },
            { (void*)T_CCE_ALPHA,   "[:alpha:]"  },
            { (void*)T_CCE_CNTRL,   "[:cntrl:]"  },
            { (void*)T_CCE_DIGIT,   "[:digit:]"  },
            { (void*)T_CCE_GRAPH,   "[:graph:]"  },
            { (void*)T_CCE_LOWER,   "[:lower:]"  },
            { (void*)T_CCE_PRINT,   "[:print:]"  },
            { (void*)T_CCE_PUNCT,   "[:punct:]"  },
            { (void*)T_CCE_SPACE,   "[:space:]"  },
            { (void*)T_CCE_BLANK,   "[:blank:]"  },
            { (void*)T_CCE_UPPER,   "[:upper:]"  },
            { (void*)T_CCE_XDIGIT,  "[:xdigit:]" },
        };
        for (size_t i = 0; i < *(&cce_tab + 1) - cce_tab; ++i) {
            if (!strcmp(C_LEXEME(lex), cce_tab[i][1]))
                { return ((long)*cce_tab[i]); }
        }
        errorf(CURRENT_LINE(lex),
                        "Bad character class expression %s.", C_LEXEME(lex));
        return (T_ERROR);
    }
    else if (found_token == T_N_CCE) {
        static void* n_cce_tab[][2] = {
            { (void*)T_N_CCE_ALNUM,     "[:^alnum:]"  },
            { (void*)T_N_CCE_ALPHA,     "[:^alpha:]"  },
            { (void*)T_N_CCE_CNTRL,     "[:^cntrl:]"  },
            { (void*)T_N_CCE_DIGIT,     "[:^digit:]"  },
            { (void*)T_N_CCE_GRAPH,     "[:^graph:]"  },
            { (void*)T_N_CCE_LOWER,     "[:^lower:]"  },
            { (void*)T_N_CCE_PRINT,     "[:^print:]"  },
            { (void*)T_N_CCE_PUNCT,     "[:^punct:]"  },
            { (void*)T_N_CCE_SPACE,     "[:^space:]"  },
            { (void*)T_N_CCE_BLANK,     "[:^blank:]"  },
            { (void*)T_N_CCE_UPPER,     "[:^upper:]"  },
            { (void*)T_N_CCE_XDIGIT,    "[:^xdigit:]" },
        };
        for (size_t i = 0; i < *(&n_cce_tab + 1) - n_cce_tab; ++i) {
            if (!strcmp(C_LEXEME(lex), n_cce_tab[i][1]))
                { return ((long)*n_cce_tab[i]); }
        }
        errorf(CURRENT_LINE(lex),
                    "Bad negate character class expression %s.", C_LEXEME(lex));
        return (T_ERROR);
    }
    lex->last_token = -1;
    return (found_token);
}

int
peek_token(lexer_t* lex) {
    if (!lex)
        { return (-1); }
    if (lex->last_token == -1)
        { lex->last_token = advance_token(lex); }
    return (lex->last_token);
}

bool
in_first(lexer_t* lex, ...) {
    va_list args;
    va_start(args, lex);
    int token;
    while ((token = va_arg(args, int)) != -1) {
        if (token == peek_token(lex))
            { return (true); }
    }
    va_end(args);
    return (false);
}

