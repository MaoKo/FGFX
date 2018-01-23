#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "regex.h"
#include "preprocess_regex.h"
#include "regex_node.h"
#include "lexical_spec.h"
#include "lexer.h"
#include "nfa.h"
#include "lexer.h"
#include "vector.h"
#include "error.h"
#include "utils.h"

#define _ONLY_STATE_TOKEN_
#include "fgfx.lex.h"
#undef  _ONLY_STATE_TOKEN_

static lexical_spec_t* regex_spec = NULL;
static spec_entry_t* regex_entry = NULL;

static regex_node_t* regex_look(void);
static regex_node_t* regex_union(void);
static regex_node_t* regex_cat(void);
static regex_node_t* regex_closure(void);
static regex_node_t* regex_not(void);
static regex_node_t* regex_atom(void);
static regex_node_t* regex_string(void);
static regex_node_t* regex_fullccl(void);
static regex_node_t* regex_loneccl(void);
static bitset_t* regex_ccl(void);
static bitset_t* regex_cce(int);
static bitset_t* regex_n_cce(int);
static regex_node_t* regex_dot(void);
static int regex_char(int);

static regex_node_t*
finite_seq(regex_node_t* root) {
    if (advance_token(regex_spec->lex) != T_DIGIT) {
        errorf(CURRENT_LINE(regex_spec->lex), "Missing a digit after the '{'.");
        return (NULL_NODE);
    }

    size_t start = atoi(C_LEXEME(regex_spec->lex));
    regex_node_t* rep_node = cpy_concat_regex_node(root, start);

    if (peek_token(regex_spec->lex) == T_REG_COMMA) {
        advance_token(regex_spec->lex);
        if (peek_token(regex_spec->lex) == T_DIGIT) {
            advance_token(regex_spec->lex);
            size_t until = atoi(C_LEXEME(regex_spec->lex));

            if (until < start) {
                del_regex_node(rep_node);
                errorf(CURRENT_LINE(regex_spec->lex),
                            "The first digit in the finite sequence %zu"
                            " is greater than the second %zu.", start, until);
                return (NULL_NODE);
            }   
            for (size_t i = start + 1; i <= until; ++i) {
                root = cpy_regex_node(root);
                rep_node = new_regex_node(AST_UNION, rep_node,
                                            cpy_concat_regex_node(root, i));
            }
        }
        else {
            rep_node = new_regex_node(AST_CONCAT, rep_node,
                new_regex_node(AST_CLOSURE, cpy_regex_node(rep_node), NULL));
        }
    }

    if (advance_token(regex_spec->lex) != T_REG_RBRACE) {
        del_regex_node(rep_node);
        errorf(CURRENT_LINE(regex_spec->lex),
                                "Missing the '}' in the finite sequence.");
        return (NULL_NODE);
    }
    return (rep_node);
}

regex_node_t*
build_regex(lexical_spec_t* spec, spec_entry_t* entry) {
    regex_spec = spec;
    regex_entry = entry;
    if (peek_token(spec->lex) == T_REG_CARET) {
        advance_token(spec->lex);
        entry->beg_line = true;
    }
    regex_node_t* root = regex_look();
    if (!root)
        { return (NULL_NODE); }
    if (peek_token(spec->lex) == T_REG_DOLLAR) {
        if (root->kind_ast == AST_LOOK) {
            del_regex_node(root);
            errorf(CURRENT_LINE(regex_spec->lex),
                                "$ can't follow the / operator.");
            return (NULL_NODE);
        }
        advance_token(spec->lex);
        root = new_regex_node(AST_LOOK, root, new_regex_node(AST_SYMBOL, '\n'));
    }
    return (root);
}

static regex_node_t*
regex_look(void) {
    regex_node_t* root = regex_union();
    if (!root)
        { return (NULL_NODE); }

    if (peek_token(regex_spec->lex) == T_REG_LOOK) {
        advance_token(regex_spec->lex);
        regex_node_t* right_op = regex_union();
        if (!right_op) {
            del_regex_node(right_op);
            return (NULL_NODE);
        }
        root = new_regex_node(AST_LOOK, root, right_op);
        regex_entry->use_look = true;
    }
    return (root);
}

static regex_node_t*
regex_union(void) {
    regex_node_t* root = regex_cat();
    if (!root)
        { return (NULL_NODE); }

    while (peek_token(regex_spec->lex) == T_REG_UNION) {
        advance_token(regex_spec->lex);
        regex_node_t* right_op = regex_cat();
        if (!right_op) {
            del_regex_node(root);
            errorf(CURRENT_LINE(regex_spec->lex), "Missing an operand to '|'.");
            return (NULL_NODE);
        }
        root = new_regex_node(AST_UNION, root, right_op);
    }
    return (root);
}

static regex_node_t*
regex_cat(void) {
    regex_node_t* root = regex_closure();
    if (!root)
        { return (NULL_NODE); }

    while (!in_first(regex_spec->lex, T_REG_LOOK, T_REG_UNION, T_REG_RPAREN,
                                    T_REG_DOLLAR, T_CLOSE_REGEX, T_EOF, -1)) {
         regex_node_t* right_op = regex_closure();
        if (!right_op) {
            del_regex_node(root);
            return (NULL_NODE); 
        }
        root = new_regex_node(AST_CONCAT, root, right_op);
    }
    return (root);
}

static regex_node_t*
regex_closure(void) {
    regex_node_t* root = regex_not();
    if (!root)
        { return (NULL_NODE); }

    while (in_first(regex_spec->lex, T_REG_STAR, T_REG_PLUS,
                                            T_REG_QUES, T_REG_LBRACE, -1)) {
        int closure_kind = advance_token(regex_spec->lex);
        switch (closure_kind) {
            case T_REG_PLUS:
                root = new_regex_node(AST_CONCAT, root,
                    new_regex_node(AST_CLOSURE, cpy_regex_node(root), NULL));
                break;
            case T_REG_QUES:
                root = new_regex_node(AST_UNION, root,
                    new_regex_node(AST_EPSILON));
                break;
            case T_REG_STAR:
                if (root->kind_ast != AST_CLOSURE)
                    { root = new_regex_node(AST_CLOSURE, root, NULL); }
                break;
            case T_REG_LBRACE: root = finite_seq(root);
                break;
        }
        if (!root)
            { return (NULL_NODE); }
    }
    return (root);
}

static regex_node_t*
regex_not(void) {
    size_t count_not = 0;

    while (peek_token(regex_spec->lex) == T_REG_NOT) {
        advance_token(regex_spec->lex);
        ++count_not;
    }

    regex_node_t* root = regex_atom();
    if (!root)
        { return (NULL_NODE); }

    // if count_not is odd
    if (count_not % 2)
        { invert_node_language(root); }

    return (root);
}

static regex_node_t*
regex_atom(void) {
    if (peek_token(regex_spec->lex) == T_REG_LBRACK)
        { return (regex_fullccl()); }
    else if (peek_token(regex_spec->lex) == T_REG_QUOTE)
        { return (regex_string()); }
    else if (peek_token(regex_spec->lex) == T_REG_LPAREN) { 
        advance_token(regex_spec->lex);
        regex_node_t* root = regex_union();

        if (advance_token(regex_spec->lex) != T_REG_RPAREN) {
            del_regex_node(root);
            errorf(CURRENT_LINE(regex_spec->lex),
                                "Missing a close paren (')').");
            return (NULL_NODE);
        }
        return (root);
    }
    else if (peek_token(regex_spec->lex) == T_REG_DOT)
        { return (regex_dot()); }
    else if (peek_token(regex_spec->lex) == T_REG_BOUND_NAME) {
        advance_token(regex_spec->lex);
        unget_char_back_buffer(LAST_LEXEME(regex_spec->lex), 1);
        return (new_regex_node(AST_BOUND_NAME,
                                strdup(C_LEXEME(regex_spec->lex) + 1)));
    }
    else {
        int kind = peek_token(regex_spec->lex);
        int val_lexeme;
        if ((val_lexeme = regex_char(kind)) != ERROR) {
            advance_token(regex_spec->lex);
            return (new_regex_node(AST_SYMBOL, val_lexeme));
        }
        else {
            errorf(CURRENT_LINE(regex_spec->lex),
                        "Unrecognized rule. Missing a char in the regex.");
            return (NULL_NODE);
        }
    }
}

static regex_node_t*
regex_string(void) {
    advance_token(regex_spec->lex);

    regex_node_t* root_concat = NULL;
    bool empty_str = true;

    while (!in_first(regex_spec->lex, T_REG_QUOTE, T_EOF, -1)) {
        int kind_char;
        if ((kind_char = advance_token(regex_spec->lex)) == T_ERROR)
            { break; }
        regex_node_t* symbol = new_regex_node(AST_SYMBOL,
                                    regex_char(kind_char));
        if (!root_concat)
            { root_concat = symbol; }
        else
            { root_concat = new_regex_node(AST_CONCAT, root_concat, symbol); }
        empty_str = true;
    }
    if (!empty_str)
        { root_concat = new_regex_node(AST_EPSILON); }
    if (!root_concat)
         { return (NULL_NODE); }
    if (advance_token(regex_spec->lex) != T_REG_QUOTE) {
        del_regex_node(root_concat);
        errorf(CURRENT_LINE(regex_spec->lex),
                                "Missing a close quote ('\"') in string.");
        return (NULL_NODE);
    }
    return (root_concat);
}

static regex_node_t*
regex_fullccl(void) {
    regex_node_t* root = regex_loneccl();
    if (!root)
        { return (NULL_NODE); }

    bool encounter_op = false;
    while (in_first(regex_spec->lex, T_REG_DIFF_CLASS,
                                T_REG_UNION_CLASS, T_REG_INTER_CLASS, -1)) {

        int kind_op = advance_token(regex_spec->lex);

        if (peek_token(regex_spec->lex) != T_REG_LBRACK) {
            del_regex_node(root);
            errorf(CURRENT_LINE(regex_spec->lex),
                                "Missing character class operand to %s.",
                                (kind_op == T_REG_DIFF_CLASS)  ? "{-}" :
                                (kind_op == T_REG_UNION_CLASS) ? "{+}" : "{&}");
            return (NULL_NODE);
        }
        else {
            regex_node_t* right_op = regex_loneccl();
            if (kind_op == T_REG_DIFF_CLASS)
                { DIFF_BITSET(root->class, right_op->class); }
            else if (kind_op == T_REG_UNION_CLASS)
                { UNION_BITSET(root->class, right_op->class); }
            else
                { INTERSECT_BITSET(root->class, right_op->class); }
            del_regex_node(right_op);
        }
        encounter_op = true;
    }
    if (is_empty_bitset(root->class)) {
        if (encounter_op) {
            warnf(CURRENT_LINE(regex_spec->lex),
                    "Generated epsilon with class operator.");
        }
        del_bitset(root->class);
        root->kind_ast = AST_EPSILON;
    }
    return (root);
}

static regex_node_t*
regex_loneccl(void) {
    advance_token(regex_spec->lex);
    bool negate = (peek_token(regex_spec->lex) == T_REG_CARET);
    if (negate)
        { advance_token(regex_spec->lex); }
    bitset_t* range = regex_ccl();
    if (!range)
        { return (NULL_NODE); }
    else if (advance_token(regex_spec->lex) != T_REG_RBRACK) {
        del_bitset(range);
        errorf(CURRENT_LINE(regex_spec->lex),
                                "Missing a close bracket (']') in ccl.");
        return (NULL_NODE);
    }

    if (is_empty_bitset(range) && !negate) {
        del_bitset(range);
        return (new_regex_node(AST_EPSILON));
    }
    else if (negate)
        { COMPL_BITSET(range); }

    return (new_regex_node(AST_CLASS, range));
}

static bitset_t*
regex_ccl(void) {
    bitset_t* range = new_bitset();
    while (!in_first(regex_spec->lex, T_REG_RBRACK, T_EOF, -1)) {
        int kind_right = advance_token(regex_spec->lex);
        if (kind_right == T_ERROR) {
            del_bitset(range);
            return (NULL_BITSET);
        }
        else if (kind_right > T_CC_FIRST && kind_right < T_CC_LAST) {
            bitset_t* (*cce_ptr)(int) = NULL;
            if (kind_right < T_CC_MIDDLE)
                { cce_ptr = &regex_cce; }
            else
                { cce_ptr = &regex_n_cce; }
            bitset_t* cce_range = (*cce_ptr)(kind_right);
            UNION_BITSET(range, cce_range);
            del_bitset(cce_range);
        }
        else {
            size_t min_range = regex_char(kind_right);

            if (peek_token(regex_spec->lex) == T_REG_HYPHEN) {
                advance_token(regex_spec->lex);
                int kind_left = advance_token(regex_spec->lex);

                size_t max_range = regex_char(kind_left);
                add_range_bitset(range, min_range, max_range + 1);

                if (max_range < min_range) {
                    warnf(CURRENT_LINE(regex_spec->lex),
                        "The min range %c is greater than the max range %c.",
                        min_range, max_range);
                }
                else if (max_range == min_range) {
                    warnf(CURRENT_LINE(regex_spec->lex),
                        "The min range and max range are both equal.");
                    ADD_BITSET(range, min_range);
                }
            }
            else
                { ADD_BITSET(range, min_range); }
        }
    }
    return (range);
}

static bitset_t*
regex_cce(int kind_cce) {
    bitset_t* cce_range = new_bitset();
    bitset_t* op_range = NULL_BITSET;

    switch (kind_cce) {
        case T_CCE_ALNUM:
            op_range = regex_cce(T_CCE_ALPHA);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            op_range = regex_cce(T_CCE_DIGIT);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            break;
        case T_CCE_ALPHA:
            op_range = regex_cce(T_CCE_LOWER);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            op_range = regex_cce(T_CCE_UPPER);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            break;
        case T_CCE_BLANK:
            ADD_BITSET(cce_range, '\t');
            ADD_BITSET(cce_range, ' ');
            break;
        case T_CCE_CNTRL:
            add_range_bitset(cce_range, '\0', '\x1F' + 1);
            ADD_BITSET(cce_range, '\x7F');
            break;
        case T_CCE_DIGIT:
            add_range_bitset(cce_range, '0', '9' + 1);
            break;
        case T_CCE_GRAPH:
            op_range = regex_cce(T_CCE_PRINT); OFF_BITSET(op_range, ' ');
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);
            break;
        case T_CCE_LOWER:
            regex_entry->use_lower = true;
            add_range_bitset(cce_range, 'a', 'z' + 1);
            break;
        case T_CCE_PRINT:
            op_range = regex_cce(T_CCE_CNTRL);
            UNION_BITSET(cce_range, COMPL_BITSET(op_range));
            del_bitset(op_range);
            break;
        case T_CCE_PUNCT:
            op_range = regex_cce(T_CCE_GRAPH);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            op_range = regex_cce(T_CCE_ALNUM);
            UNION_BITSET(cce_range, COMPL_BITSET(op_range));
            del_bitset(op_range);        
            break;
        case T_CCE_SPACE:
            add_range_bitset(cce_range, '\t', '\r' + 1);
            ADD_BITSET(cce_range, ' ');
            break;
        case T_CCE_UPPER:
            regex_entry->use_upper = true;
            add_range_bitset(cce_range, 'A', 'Z' + 1);
            break;
        case T_CCE_XDIGIT:
            op_range = regex_cce(T_CCE_DIGIT);
            UNION_BITSET(cce_range, op_range); del_bitset(op_range);        
            add_range_bitset(cce_range, 'a', 'f' + 1);
            add_range_bitset(cce_range, 'A', 'F' + 1);
            break;
    }
    return (cce_range);
}

static bitset_t*
regex_n_cce(int kind_cce) {
    switch (kind_cce) {
        case T_N_CCE_ALNUM:
            return (COMPL_BITSET(regex_cce(T_CCE_ALNUM)));
        case T_N_CCE_ALPHA:
            return (COMPL_BITSET(regex_cce(T_CCE_ALPHA)));
        case T_N_CCE_BLANK:
            return (COMPL_BITSET(regex_cce(T_CCE_BLANK)));
        case T_N_CCE_CNTRL:
            return (COMPL_BITSET(regex_cce(T_CCE_CNTRL)));
        case T_N_CCE_DIGIT:
            return (COMPL_BITSET(regex_cce(T_CCE_DIGIT)));
        case T_N_CCE_GRAPH:
            return (COMPL_BITSET(regex_cce(T_CCE_GRAPH)));
        case T_N_CCE_LOWER:
            return (COMPL_BITSET(regex_cce(T_CCE_LOWER)));
        case T_N_CCE_PRINT:
            return (COMPL_BITSET(regex_cce(T_CCE_PRINT)));
        case T_N_CCE_PUNCT:
            return (COMPL_BITSET(regex_cce(T_CCE_PUNCT)));
        case T_N_CCE_SPACE:
            return (COMPL_BITSET(regex_cce(T_CCE_SPACE)));
        case T_N_CCE_UPPER:
            return (COMPL_BITSET(regex_cce(T_CCE_UPPER)));
        case T_N_CCE_XDIGIT:
            return (COMPL_BITSET(regex_cce(T_CCE_XDIGIT)));
        }
    return (NULL_BITSET);
}

static regex_node_t*
regex_dot(void) {
    advance_token(regex_spec->lex);
    bitset_t* newl = new_bitset();
    ADD_BITSET(newl, '\n');
    return (new_regex_node(AST_CLASS, COMPL_BITSET(newl)));
}

static int
hex_to_dec(int c) {
    if (isdigit(c)) { return (c - '0'); }
    else if (islower(c)) { return ((c - 'a') + 10); }
    else if (isupper(c)) { return ((c - 'A') + 10); }
    return (ERROR);
}

static int
regex_char(int kind) {
    if (kind == T_REG_CHAR) {
        size_t input_c = (*C_LEXEME(regex_spec->lex));
        if (input_c == '\\') {
            input_c = C_LEXEME(regex_spec->lex)[1];

            switch (input_c) {
                case 'n': input_c = '\n'; break;
                case 'a': input_c = '\a'; break;
                case 'b': input_c = '\b'; break;
                case 'f': input_c = '\f'; break;
                case 'r': input_c = '\r'; break;
                case 't': input_c = '\t'; break;
                case 'v': input_c = '\v'; break;
                default:  break;
            }
        }
        return (input_c);
    }
    else if ( kind == T_OCT_NUM) {
        char const* first_digit = C_LEXEME(regex_spec->lex) + 1;
        size_t oct_num = 0;
        while (*first_digit) {
            oct_num = (*first_digit - '0') + (oct_num * 8);
            ++first_digit;
        }
        return (oct_num);
    }
    else if ( kind == T_HEX_NUM) {
        char const* first_digit = C_LEXEME(regex_spec->lex) + 2;
        size_t hex_num = 0;
        while (*first_digit) {
            hex_num = hex_to_dec(*first_digit) + (hex_num * 16);
            ++first_digit;
        }
        return (hex_num);
    }
    return (ERROR);
}

