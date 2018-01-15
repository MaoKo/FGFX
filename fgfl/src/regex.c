#include <stdio.h>
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

static lexical_spec_t* local_spec = NULL;

// Static Prototype

static regex_node_t* regex_look(void);
static regex_node_t* regex_union(void);
static regex_node_t* regex_cat(void);
static regex_node_t* regex_closure(void);
static regex_node_t* regex_atom(void);
static regex_node_t* regex_string(void);
static regex_node_t* regex_fullccl(void);
static regex_node_t* regex_loneccl(void);
static bitset_t* regex_ccl(void);
static bitset_t* regex_cce(int);
static bitset_t* regex_n_cce(int);
static regex_node_t* regex_dot(void);

#if 0
static bool is_oct_prefix(int);
static bool is_hex_prefix(int);
static int read_oct(void);
static int read_hex(void);
#endif

// Input Function

static size_t
eval_lexeme_char(void) {
	size_t input_c = *C_LEXEME(local_spec->lex);
	if (input_c == '\\') {
        input_c = C_LEXEME(local_spec->lex)[1];
#if 0
		if (is_oct_prefix(input_c))
			{ return (read_oct()); }
		else if (is_hex_prefix(input_c))
			{ return (read_hex()); }
#endif

		switch (input_c) {
			case 'n': input_c = '\n'; break;
			case 'a': input_c = '\a'; break;
			case 'b': input_c = '\b'; break;
			case 'f': input_c = '\f'; break;
			case 'r': input_c = '\r'; break;
			case 't': input_c = '\t'; break;
			case 'v': input_c = '\v'; break;
			case '\n':
				break;
			default:  break;
		}
	}
	return (input_c);
}

// Predicate Char Function

#if 0
static bool
is_oct_prefix(int c) {
	return (c == 'o' || c == 'O');
}

static bool
is_oct(int c) {
	return (c >= '0' && c <= '7');
}

static bool
is_hex_prefix(int c) {
	return (c == 'x' || c == 'X');
}

static bool
is_hex(int c) {
	return ((c >= '0' && c <= '9') ||
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z'));
}

static int
hex2dec(int c) {
	if (isdigit(c)) { return (c - '0'); }
	else if (islower(c)) { return ((c - 'a') + 10); }
	else if (isupper(c)) { return ((c - 'A') + 10); }
	return (-1);
}

static int
read_oct(void) {
	if (!is_oct(peek()))
		{ /* ERROR */ }
	int oct_num = advance() - '0';
	if (is_oct(peek())) { oct_num = (advance() - '0') + (oct_num * 8); }
	return (oct_num);
}

static int
read_hex(void) {
	if (!is_hex(peek()))
		{ /* ERROR */ }
	int hex_num = hex2dec(advance());
	if (is_hex(peek()))
		{ hex_num = hex2dec(advance()) + (hex_num * 16); }
	return (hex_num);
}
#endif

// Regex Function

static regex_node_t*
finite_seq(regex_node_t* root) {
    if (advance_token(local_spec->lex) != T_DIGIT) {
        errorf(CURRENT_LINE(local_spec->lex), "Missing a digit after the '{'.");
        return (NULL_NODE);
    }

	size_t start = atoi(C_LEXEME(local_spec->lex));
	regex_node_t* rep_node = cpy_concat_regex_node(root, start);

	if (peek_token(local_spec->lex) == T_REG_COMMA) {
        advance_token(local_spec->lex);
		if (peek_token(local_spec->lex) == T_DIGIT) {
            advance_token(local_spec->lex);
			size_t until = atoi(C_LEXEME(local_spec->lex));

			if (until < start) {
                del_regex_node(rep_node);
                errorf(CURRENT_LINE(local_spec->lex),
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

    if (advance_token(local_spec->lex) != T_REG_RBRACE) {
        del_regex_node(rep_node);
        errorf(CURRENT_LINE(local_spec->lex),
                                "Missing the '}' in the finite sequence.");
        return (NULL_NODE);
    }
	return (rep_node);
}

regex_node_t*
build_regex(lexical_spec_t* spec) {
    local_spec = spec;
    // TODO anchor ( '^' and '$')
	return (regex_look());
}

static regex_node_t*
regex_look(void) {
	regex_node_t* root = regex_union();
    if (!root)
        { return (NULL_NODE); }

	if (peek_token(local_spec->lex) == T_REG_LOOK) {
		advance_token(local_spec->lex);
        regex_node_t* right_op = regex_union();
        if (!right_op) {
            del_regex_node(right_op);
            return (NULL_NODE);
        }
		root = new_regex_node(AST_CONCAT, root, right_op);
		root->look_sym = true;
	}
	return (root);
}

static regex_node_t*
regex_union(void) {
	regex_node_t* root = regex_cat();
    if (!root)
        { return (NULL_NODE); }

	while (peek_token(local_spec->lex) == T_REG_UNION) {
		advance_token(local_spec->lex);
        regex_node_t* right_op = regex_cat();
        if (!right_op) {
            del_regex_node(root);
            errorf(CURRENT_LINE(local_spec->lex), "Missing an operand to '|'.");
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

	while (!in_first(local_spec->lex, T_REG_LOOK, T_REG_UNION,
                                    T_REG_RPAREN, T_END_REGEX, T_EOF, -1)) {
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
	regex_node_t* root = regex_atom();
    if (!root)
        { return (NULL_NODE); }

	while (in_first(local_spec->lex, T_REG_STAR, T_REG_PLUS,
                                            T_REG_QUES, T_REG_LBRACE, -1)) {
		int closure_kind = advance_token(local_spec->lex);
		switch (closure_kind) {
			case T_REG_PLUS:
				root = new_regex_node(AST_CONCAT, root,
                    new_regex_node(AST_CLOSURE, cpy_regex_node(root), NULL));
				break;
			case T_REG_QUES:
				root = new_regex_node(AST_UNION, root,
				    new_regex_node(AST_SYMBOL, ALONE_S, EPSILON));
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
regex_atom(void) {
    if (peek_token(local_spec->lex) == T_REG_LBRACK)
        { return (regex_fullccl()); }
	else if (peek_token(local_spec->lex) == T_REG_QUOTE)
        { return (regex_string()); }
	else if (peek_token(local_spec->lex) == T_REG_LPAREN) { 
        advance_token(local_spec->lex);
		regex_node_t* root = regex_union();

        if (advance_token(local_spec->lex) != T_REG_RPAREN) {
            del_regex_node(root);
            errorf(CURRENT_LINE(local_spec->lex),
                                "Missing a close paren (')').");
            return (NULL_NODE);
        }
		return (root);
	}
	else if (peek_token(local_spec->lex) == T_REG_DOT)
        { return (regex_dot()); }
    else if (peek_token(local_spec->lex) == T_REG_BOUND_NAME) {
        advance_token(local_spec->lex);
		unget_char_back_buffer(LAST_LEXEME(local_spec->lex), 1);
		return (new_regex_node(AST_BOUND_NAME,
                                strdup(C_LEXEME(local_spec->lex) + 1)));
    }
    else if (peek_token(local_spec->lex) == T_REG_CHAR) {
        advance_token(local_spec->lex);
		return (new_regex_node(AST_SYMBOL, ALONE_S, eval_lexeme_char()));
    }
    else {
        errorf(CURRENT_LINE(local_spec->lex), "Missing a char in the regex.");
        return (NULL_NODE);
    }
}

static regex_node_t*
regex_string(void) {
    advance_token(local_spec->lex);

	regex_node_t* root_concat = NULL;
    bool empty_str = true;

	while (!in_first(local_spec->lex, T_REG_QUOTE, T_EOF, -1)) {
        if (advance_token(local_spec->lex) == T_ERROR)
            { break; }
   		regex_node_t* symbol = new_regex_node(AST_SYMBOL,
                                    ALONE_S, eval_lexeme_char());
		if (!root_concat)
			{ root_concat = symbol; }
		else
			{ root_concat = new_regex_node(AST_CONCAT, root_concat, symbol); }
        empty_str = true;
	}
	if (!empty_str)
		{ root_concat = new_regex_node(AST_SYMBOL, ALONE_S, EPSILON); }
    if (!root_concat)
         { return (NULL_NODE); }
    if (advance_token(local_spec->lex) != T_REG_QUOTE) {
        del_regex_node(root_concat);
        errorf(CURRENT_LINE(local_spec->lex),
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

    while (in_first(local_spec->lex, T_REG_DIFF_CLASS, T_REG_UNION_CLASS, -1)) {
        int kind_op = advance_token(local_spec->lex);
        if (peek_token(local_spec->lex) != T_REG_LBRACK) {
            del_regex_node(root);
            errorf(CURRENT_LINE(local_spec->lex),
                                "Missing character class operand to %s.",
                                (kind_op == T_REG_DIFF_CLASS) ? "{-}" : "{+}");
            return (NULL_NODE);
        }
        else {
            regex_node_t* right_op = regex_loneccl();
            if (kind_op == T_REG_DIFF_CLASS)
        	    { DIFF_BITSET(root->cclass, right_op->cclass); }
            else
        	    { UNION_BITSET(root->cclass, right_op->cclass); }
            del_regex_node(right_op);
        }
    }
    return (root);
}

static regex_node_t*
regex_loneccl(void) {
    advance_token(local_spec->lex);
	bool negate = (peek_token(local_spec->lex) == T_REG_CARET);
    if (negate)
        { advance_token(local_spec->lex); }
    bitset_t* range = regex_ccl();
    if (!range)
        { return (NULL_NODE); }
    else if (advance_token(local_spec->lex) != T_REG_RBRACK) {
        del_bitset(range);
        errorf(CURRENT_LINE(local_spec->lex),
                                "Missing a close bracket (']') in ccl.");
        return (NULL_NODE);
    }

	if (is_empty_bitset(range) && !negate) {
		del_bitset(range);
		return (new_regex_node(AST_SYMBOL, ALONE_S, EPSILON));
	}
	else if (negate)
		{ COMPL_BITSET(range); }

	return (new_regex_node(AST_SYMBOL, MULTI_S, range));
}

static bitset_t*
regex_ccl(void) {
	bitset_t* range = new_bitset();
	while (!in_first(local_spec->lex, T_REG_RBRACK, T_EOF, -1)) {
        int kind = advance_token(local_spec->lex);
        if (kind == T_ERROR) {
            del_bitset(range);
            return (NULL_BITSET);
        }
        else if (kind != T_REG_CHAR && kind != T_REG_HYPHEN) {
            bitset_t* (*cce_ptr)(int) = NULL;
            if (kind == T_CCE)
                { cce_ptr = &regex_cce; }
            else
                { cce_ptr = &regex_n_cce; }
            bitset_t* cce_range = (*cce_ptr)(kind);
            UNION_BITSET(range, cce_range);
            del_bitset(cce_range);
        }
        else {
            size_t min_range = eval_lexeme_char();

            if (peek_token(local_spec->lex) == T_REG_HYPHEN) {
                advance_token(local_spec->lex);
                advance_token(local_spec->lex);

                size_t max_range = eval_lexeme_char();
                add_range_bitset(range, min_range, max_range + 1);

                if (max_range < min_range) {
                    warnf(CURRENT_LINE(local_spec->lex),
                        "The min range %c is greater than the max range %c.",
                        min_range, max_range);
                }
                else if (max_range == min_range) {
                    warnf(CURRENT_LINE(local_spec->lex),
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
            add_range_bitset(cce_range, '\t', '\n' + 1);
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
    advance_token(local_spec->lex);
	bitset_t* newl = new_bitset();
	ADD_BITSET(newl, '\n');
	return (new_regex_node(AST_SYMBOL, MULTI_S, COMPL_BITSET(newl)));
}
