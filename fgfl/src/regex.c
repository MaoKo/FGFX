#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "regex.h"
#include "tokendef.h"
#include "lexer.h"
#include "utils.h"
#include "nfa.h"
#include "vector.h"

static token_spec_t* local_spec = NULL;
static bool escaped = false;

// Static Prototype

static node_ast_t* reg_expr(void);
static node_ast_t* reg_term(void);
static node_ast_t* reg_fact(void);
static node_ast_t* reg_atom(void);
static node_ast_t* reg_quote(void);
static node_ast_t* reg_range(void);

static bool is_oct_prefix(int);
static bool is_hex_prefix(int);
static int read_oct(void);
static int read_hex(void);

static int get_c(void);

// Node Function

static node_ast_t*
node_ast(int kind, ...) {
	node_ast_t* node = NEW(node_ast_t, 1);
	if (!node)
		{ return (NULL); }
	va_list args;
	va_start(args, kind);
	if (kind != AST_SYMBOL) {
		node->left = va_arg(args, node_ast_t*);
		node->right = va_arg(args, node_ast_t*);
	}
	else {
		node->alone = va_arg(args, int);
		if (node->alone)
			{ node->symbol = va_arg(args, int); }
		else
			{ node->cclass = va_arg(args, bitset_t*); }
	}
	node->kind_ast = kind;
	va_end(args);
	return (node);
}

void
del_node_ast(node_ast_t* node) {
	if (node) {
		if (node->kind_ast != AST_SYMBOL) {
			del_node_ast(node->left);
			del_node_ast(node->right);
		}
		else if (!node->alone)
			{ del_bitset(node->cclass); }
		FREE(node);
	}
}

static node_ast_t*
cpy_node_ast(node_ast_t* root) {
	if (root) {
		if (root->kind_ast != AST_SYMBOL) {
			node_ast_t* left = cpy_node_ast(root->left);
			node_ast_t* right = cpy_node_ast(root->right);
			return (node_ast(root->kind_ast, left, right));
		}
		else { 
			if (root->alone) {
				return (node_ast(AST_SYMBOL,
						ALONE_S, root->symbol)); }
			else {
				return (node_ast(AST_SYMBOL, MULTI_S,
						dup_bitset(root->cclass)));
			}
		}
	}
	return (NULL);
}

static node_ast_t*
cpy_concat_node_ast(node_ast_t* root, size_t size) {
	if (!size)
		{ return (node_ast(AST_SYMBOL, EPSILON)); }
	node_ast_t* atom = root;
	for (size_t i = 1; i < size; ++i)
		{ root = node_ast(AST_CONCAT, root, cpy_node_ast(atom)); }
	return (root);
}

// Input Function

static int
read_c(void) {
	char input_c = '\0';
	int eof = read(FILDE_LEX(local_spec->lex), &input_c, 1);
	if (!eof)
		{ return (EOF); }
	return (input_c);
}

static int
skip_until(int(*predF)(int), int last) {
	int c = 0;
	if ((last != -1) && !((*predF)(last)))
		{ return (last);  }
	do {
		if ((c = get_c()) == EOF)
			{ return (EOF); }
	} while ((*predF)(c));
	return (c);
}

static inline void
skip_front_whitespace(int last) {
	local_spec->lex->last_char = skip_until(&isspace, last);
}

static inline int
is_tab_or_space(int c) {
	return (c == '\t' || c == ' ');
}

static int
get_c(void) {
	int input_c = read_c();
	escaped = false;

	if (input_c == EOF)
		{ return (EOF); }
	else if (input_c == '\\') {
		input_c = read_c();
		escaped = true;
	
		if (is_oct_prefix(input_c))
			{ return (read_oct()); }
		else if (is_hex_prefix(input_c))
			{ return (read_hex()); }

		switch (input_c) {
			case 'n': input_c = '\n'; break;
			case 'a': input_c = '\a'; break;
			case 'b': input_c = '\b'; break;
			case 'f': input_c = '\f'; break;
			case 'r': input_c = '\r'; break;
			case 't': input_c = '\t'; break;
			case 'v': input_c = '\v'; break;
			case '\n':
				++CURRENT_LINE(local_spec->lex);
				input_c = skip_until(&is_tab_or_space, -1);
				break;
			case EOF: /* ERROR */ ; break;
			default: break;
		}
	}
	return (input_c);
}

static int
peek(void) {
	if (local_spec->lex->last_char == -1)
		{ local_spec->lex->last_char = get_c(); }
	return (local_spec->lex->last_char);
}

static inline int
advance(void) {
	int old_peek = peek();
	local_spec->lex->last_char = get_c();
	return (old_peek);
}

static bool
match(int symbl) {
	int is_match = (peek() == symbl);
	if (is_match)
		{ advance(); }
	return (is_match);
}

static void
expected(char expect_c) {
	if (!match(expect_c))
		{ /* ERROR */ }
}

static inline bool
is_end_input(void) {
	return (peek() == EOF);
}

static inline bool
is_end_regex(void) {
	return ((isspace(peek()) || peek() == ';') && !escaped);
}

static inline bool
is_end(void) {
	return (is_end_input() || is_end_regex());
}

// Predicate Char Function

static bool
is_metachar(int symbl) {
	switch (symbl) {
		case REG_UNION: case REG_LPAREN: case REG_RPAREN:
		case REG_LBRACK: case REG_KLEENE: case REG_PKLEENE:
		case REG_OKLEENE: case REG_LBRACE: case REG_DQUOTE:
		case REG_DOT: case REG_SEMI: return (!escaped);
	}
	return (false);
} 

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

static inline bool
is_letter(int c) {
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c == '_'));
}

// Regex Function

static int
str2int(void) {
	int digit = 0;
	while (isdigit(peek()))
		{ digit = (digit + (advance() - '0')) + (digit * 10); }
	return (digit);
}

static node_ast_t*
diff_range(node_ast_t* root) {
	if (root->alone != MULTI_S)
		{ /* ERROR */ }
	expected(REG_RBRACE);
	if (!match(REG_LBRACK))
		{ /* ERROR */ }
	node_ast_t* diff = reg_range();
	if (diff->alone != MULTI_S)
		{ /* ERROR */ }
	DIFF_BITSET(root->cclass, diff->cclass);
	return (root);
}

static buffer_t*
read_ident(void) {
	buffer_t* buf = new_buffer();
	while (is_letter(peek()) || isdigit(peek()))
		{ write_char_buffer(buf, advance()); }
	return (buf);
}

static int
cmp_entry_str(token_entry_t const* entry, char const* str) {
	return (strcmp(entry->name, str));
}

static node_ast_t*
bound_name(node_ast_t* root) {
	node_ast_t* rep_node = NULL;
	buffer_t* bound = read_ident();
	
	int index = get_index_vector(local_spec->entry_lst,
		BODY_BUFFER(bound), &cmp_entry_str);
	if (index != -1) {
		token_entry_t* entry = (token_entry_t*)
				AT_VECTOR(local_spec->entry_lst, index);
		rep_node = cpy_node_ast(entry->reg);
		USED_ENTRY(entry);
	}
	if (rep_node)
		{ rep_node = node_ast(AST_CONCAT, root, rep_node); }
	else {
		fprintf(stderr, "Error (%d): Undefined name %s.\n",
			CURRENT_LINE(local_spec->lex), BODY_BUFFER(bound));
	}

	del_buffer(bound);
	return (rep_node);
}

static node_ast_t*
finite_seq(node_ast_t* root) {
	int start = str2int();
	node_ast_t* rep_node = cpy_concat_node_ast(root, start);
	if (match(REG_COMMA)) {
		if (isdigit(peek())) {
			int until = str2int();
			if (until < start)
				{ /* ERROR */ }	
			for (int i = start + 1; i <= until; ++i) {
				root = cpy_node_ast(root);
				rep_node = node_ast(AST_UNION, rep_node,
						cpy_concat_node_ast(root, i));
			}
		}
		else {
			rep_node = node_ast(AST_CONCAT, rep_node,
					node_ast(AST_CLOSURE,
					cpy_node_ast(rep_node), NULL));
		}
	}
	return (rep_node);
}

static node_ast_t*
curly_regex(node_ast_t* root) {
	node_ast_t* rep_node = NULL;
	if (match(REG_HYPHEN))
		{ return (diff_range(root));  }
	else if (is_letter(peek()))
		{ rep_node = bound_name(root); }
	else if (isdigit(peek()))
		{ rep_node = finite_seq(root); }
	expected(REG_RBRACE);
	return (rep_node);
}

/*
static set_t*
complement_negate_range(set_t* base) {
	set_t* compl = EMPTY_SET;
	for (long i = MIN_ASCII; i < MAX_ASCII; ++i) {
		if (!has_set(base, (void*)i))
			{ add_set(&compl, (void*)i); }
	}
	del_set(base);
	return (compl);
}
*/

static inline node_ast_t*
dot_regex(void) {
	bitset_t* newl = new_bitset();
	ADD_BITSET(newl, '\n');
	return (node_ast(AST_SYMBOL, MULTI_S, COMPL_BITSET(newl)));
}

node_ast_t*
regex2ast(token_spec_t* spec) {
	if (!spec)
		{ return (NULL); }
	local_spec = spec;
	skip_front_whitespace(local_spec->lex->last_char);
	return (reg_expr());
}

static bool
is_in_follow(int alone, ...) {
	va_list arg;
	va_start(arg, alone);
	bool is_match = (peek() == alone);
	while (!is_match) {
		int next = va_arg(arg, int);
		if (next == -1)
			{ return (false); }
		is_match = (peek() == next);
	}

	va_end(arg);
	return (is_match && !escaped);
}

static node_ast_t*
reg_expr(void) {
	node_ast_t* root = reg_term();
	while (is_in_follow(REG_UNION, -1)) {
		advance();
		root = node_ast(AST_UNION, root, reg_term());
	}
	return (root);
}

static node_ast_t*
reg_term(void) {
	node_ast_t* root = reg_fact();
	while (!is_in_follow(REG_UNION, REG_RPAREN, -1) && !is_end())
		{ root = node_ast(AST_CONCAT, root, reg_fact()); }
	return (root);
}

static node_ast_t*
reg_fact(void) {
	node_ast_t* root = reg_atom();
	while (is_in_follow(REG_KLEENE, REG_PKLEENE,
					REG_OKLEENE, REG_LBRACE, -1)) {
		int closure_kind = advance();
		switch (closure_kind) {
			case REG_PKLEENE:
				root = node_ast(AST_CONCAT, root,
						node_ast(AST_CLOSURE,
						cpy_node_ast(root), NULL));
				break;
			case REG_OKLEENE:
				root = node_ast(AST_UNION, root,
						node_ast(AST_SYMBOL, ALONE_S,
						EPSILON));
				break;
			case REG_KLEENE:
				root = node_ast(AST_CLOSURE, root, NULL);
				break;
			case REG_LBRACE: root = curly_regex(root);
				break;
		}
	}
	return (root);
}

static node_ast_t*
reg_atom(void) {
	//printf("Peek = %c\n", peek());
	if (!is_metachar(peek()))
		{ return (node_ast(AST_SYMBOL, ALONE_S, advance())); }
	else if (match(REG_LPAREN)) { 
		node_ast_t* expr = reg_expr();
		expected(REG_RPAREN);
		return (expr);
	}
	else if (match(REG_LBRACK)) { return (reg_range()); }
	else if (match(REG_DOT)) { return (dot_regex()); }
	else if (match(REG_DQUOTE)) { return (reg_quote()); }
	return (node_ast(AST_SYMBOL, ALONE_S, EPSILON));
}

static node_ast_t*
reg_quote(void) {
	node_ast_t* root_concat = NULL;
	while (!is_in_follow(REG_DQUOTE, -1) && !is_end_input()) {
		int c = advance();
		node_ast_t* symbol = node_ast(AST_SYMBOL, ALONE_S, c);
		if (!root_concat)
			{ root_concat = symbol; }
		else
			{ root_concat = node_ast(AST_CONCAT, root_concat, symbol); }
	}
	advance();
	if (!root_concat)
		{ root_concat = node_ast(AST_SYMBOL, ALONE_S, EPSILON); }
	expected(REG_DQUOTE);
	return (root_concat);
}

static node_ast_t*
reg_range(void) {
	bool negate = false;
	bitset_t* range = new_bitset();
	if (match(REG_CARET))
		{ negate = true; }
	int last = 0;
	bool in_range = true;
	while (!is_in_follow(REG_RBRACK, -1) && !is_end_input()) {
		long crt_c = advance();
		if (crt_c == '-' && !in_range &&
				(peek() != REG_RBRACK && !is_end())) {
			crt_c = advance();
			for (long i = last + 1; i <= crt_c; ++i)
				{ ADD_BITSET(range, i); }
			in_range = true;
		}
		else {
			ADD_BITSET(range, crt_c);
			in_range = false;
		}
		last = crt_c;
	}
	advance();
	if (is_empty_bitset(range) && !negate) {
		del_bitset(range);
		return (node_ast(AST_SYMBOL, ALONE_S, EPSILON));
	}

	if (negate)
		{ COMPL_BITSET(range); }
	expected(REG_RBRACK);
	return (node_ast(AST_SYMBOL, MULTI_S, range));
}
