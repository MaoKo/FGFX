#include "regex.h"

static char const* parse_stream = NULL;
static vector_t const* crt_symbol = NULL;

static node_ast_t* node_ast(int kind, ...) {
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
			{ node->cclass = va_arg(args, set_t*); }
	}
	node->kind_ast = kind;
	va_end(args);
	return (node);
}

void del_node_ast(node_ast_t* node) {
	if (node) {
		if (node->kind_ast != AST_SYMBOL) {
			del_node_ast(node->left);
			del_node_ast(node->right);
		}
		else if (!node->alone)
			{ del_set(node->cclass); }
		FREE(node);
	}
}

static node_ast_t* cpy_node_ast(node_ast_t* root) {
	if (root) {
		if (root->kind_ast != AST_SYMBOL) {
			node_ast_t* left = cpy_node_ast(root->left);
			node_ast_t* right = cpy_node_ast(root->right);
			return (node_ast(root->kind_ast, left, right));
		}
		else { 
			if (root->alone)
				{ return (node_ast(AST_SYMBOL, ALONE_S, root->symbol)); }
			else {
				return (node_ast(AST_SYMBOL, MULTI_S,
							cpy_set(root->cclass)));
			}
		}
	}
	return (NULL);
}

static node_ast_t* cpy_concat_node_ast(node_ast_t* root, size_t size) {
	if (!size)
		{ return (node_ast(AST_SYMBOL, EPSILON)); }
	node_ast_t* atom = root;
	for (size_t i = 1; i < size; ++i)
		{ root = node_ast(AST_CONCAT, root, cpy_node_ast(atom)); }
	return (root);
}

static bool match(int symbl) {
	if ((unsigned)*parse_stream == (unsigned)symbl) {
		if (*parse_stream)
			{ ++parse_stream; }
		return (true);
	}
	return (false);
}

static int peek(void) {
	return ((unsigned)*parse_stream);
}

static bool is_end(void) {
	return (!*parse_stream);
}

static bool is_metachar(int symbl) {
	switch (symbl) {
		case REG_UNION: case REG_LPAREN: case REG_RPAREN:
		case REG_LBRACK: case REG_KLEENE: case REG_PKLEENE:
		case REG_OKLEENE: case REG_LBRACE: return (true);
	}
	return (false);
} 

static int get_c(void) {
	int input_c;
	if (*parse_stream == EOS)
		{ return (EOS); }
	else if (match(REG_ESCAPE)) {
		input_c = *parse_stream++;
		switch (input_c) {
			case 'n': input_c = '\n'; break; case 'a': input_c = '\a'; break;
			case 'b': input_c = '\b'; break; case 'f': input_c = '\f'; break;
			case 'r': input_c = '\r'; break; case 't': input_c = '\t'; break;
			case 'v': input_c = '\v'; break; case EOS: /* ERROR */ ; break;
			default: break;
		}
	}
	else
		{ input_c = (unsigned)*parse_stream++; }
	return (input_c);
}

static node_ast_t* expected_char(node_ast_t* root, char expect_c) {
	if (!match(expect_c))
		{ /* ERROR */ }
	return (root);
}

static int str2int(void) {
	int digit = 0;
	while (isdigit(peek()))
		{ digit = (digit + (get_c() - '0')) + (digit * 10); }
	return (digit);
}

static node_ast_t* finite_seq(node_ast_t* root) {
	node_ast_t* rep_node = NULL;
	if (!isdigit(peek())) {
		for (size_t i = 0; i < size_vector(crt_symbol); ++i) {
			token_entry_t* en = (token_entry_t*)at_vector(crt_symbol, i);
			if (!strncmp(parse_stream, en->name, strlen(en->name))) {
				parse_stream += strlen(en->name);
				rep_node = cpy_node_ast(en->reg);
				break;
			}
		}
		if (rep_node)
			{ rep_node = node_ast(AST_CONCAT, root, rep_node); }
	}
	else {
		int start = str2int();
		rep_node = cpy_concat_node_ast(root, start);
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
	}
	return (expected_char(rep_node, REG_RBRACE));
}

static set_t* complement_negate_range(set_t* base) {
	set_t* compl = EMPTY_SET;
	for (long i = MIN_ASCII; i < MAX_ASCII; ++i) {
		if (!has_set(base, (void*)i))
			{ add_set(&compl, (void*)i); }
	}
	del_set(base);
	return (compl);
}

static inline node_ast_t* dot_regex(void) {
	set_t* nl = EMPTY_SET;
	add_set(&nl, (void*)'\n');
	return (node_ast(AST_SYMBOL, MULTI_S, complement_negate_range(nl)));
}

static node_ast_t* reg_expr(void);
static node_ast_t* reg_term(void);
static node_ast_t* reg_fact(void);
static node_ast_t* reg_atom(void);
static node_ast_t* reg_quote(void);
static node_ast_t* reg_range(void);

node_ast_t* regex2ast(char const* regexstr, vector_t const* symbol) {
	if (!regexstr)
		{ return (NULL); }
	parse_stream = regexstr;
	crt_symbol = symbol;
	return (reg_expr());
}

static node_ast_t* reg_expr(void) {
	node_ast_t* root = reg_term();
	while (match(REG_UNION))
		{ root = node_ast(AST_UNION, root, reg_term()); }
	return (root);
}

static node_ast_t* reg_term(void) {
	node_ast_t* root = reg_fact();
	while (peek() != REG_UNION && peek() != REG_RPAREN && !is_end())
		{ root = node_ast(AST_CONCAT, root, reg_fact()); }
	return (root);
}

static inline bool follow_fact(void) {
	return (peek() == REG_KLEENE || peek() == REG_PKLEENE ||
			peek() == REG_OKLEENE || peek() == REG_LBRACE);
}

static node_ast_t* reg_fact(void) {
	node_ast_t* root = reg_atom();
	while (follow_fact()) {
		int closure_kind = get_c();
		switch (closure_kind) {
			case REG_PKLEENE:
				root = node_ast(AST_CONCAT, root,
						node_ast(AST_CLOSURE,
						cpy_node_ast(root), NULL));
				break;
			case REG_OKLEENE:
				root = node_ast(AST_UNION, root, node_ast(AST_SYMBOL,
							ALONE_S, EPSILON));
				break;
			case REG_KLEENE:
				root = node_ast(AST_CLOSURE, root, NULL);
				break;
			case REG_LBRACE: root = finite_seq(root);
				break;
		}
	}
	return (root);
}

static node_ast_t* reg_atom(void) {
	if (match(REG_LPAREN)) { return (expected_char(reg_expr(), REG_RPAREN)); }
	else if (match(REG_LBRACK)) { return (expected_char(reg_range(), REG_RBRACK)); }
	else if (match(REG_DOT)) { return (dot_regex()); }
	else if (match(DQUOTE))	{ return (expected_char(reg_quote(), DQUOTE)); }
	else if (!is_metachar(peek()))
		{ return (node_ast(AST_SYMBOL, ALONE_S, get_c())); }
	return (node_ast(AST_SYMBOL, ALONE_S, EPSILON));
}

static node_ast_t* reg_quote(void) {
	node_ast_t* root_concat = NULL;
	while (peek() != DQUOTE && !is_end()) {
		int c = get_c();
		node_ast_t* symbol = node_ast(AST_SYMBOL, ALONE_S, c);
		if (!root_concat)
			{ root_concat = symbol; }
		else
			{ root_concat = node_ast(AST_CONCAT, root_concat, symbol); }
	}
	if (!root_concat)
		{ root_concat = node_ast(AST_SYMBOL, ALONE_S, EPSILON); }
	return (root_concat);
}

static node_ast_t* reg_range(void) {
	bool negate = false;
	set_t* range = EMPTY_SET;
	if (match(REG_CARET))
		{ negate = true; }
	int last;
	bool in_range = true;
	while (peek() != REG_RBRACK && !is_end()) {
		long crt_c = get_c();
		if (crt_c == '-' && !in_range &&
				(peek() != REG_RBRACK && !is_end())) {
			crt_c = get_c();
			for (long i = last + 1; i <= crt_c; ++i)
				{ add_set(&range, (void*)i); }
			in_range = true;
		}
		else {
			add_set(&range, (void*)crt_c);
			in_range = false;
		}
		last = crt_c;
	}
	if (range == EMPTY_SET)
		{ add_set(&range, (void*)EPSILON); }

	if (negate)
		{ range = complement_negate_range(range); }
	return (node_ast(AST_SYMBOL, MULTI_S, range));
}
