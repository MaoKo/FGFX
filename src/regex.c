#include "regex.h"

static char const* parse_stream = NULL;
static vector_t const* crt_symbol = NULL;

static node_ast_t* reg_expr(void);
static node_ast_t* reg_term(void);
static node_ast_t* reg_fact(void);
static node_ast_t* reg_atom(void);
static node_ast_t* reg_quote(void);
static node_ast_t* reg_range(void);
static int get_c(void);

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
		case REG_OKLEENE: case REG_LBRACE: case REG_DQUOTE:
		case REG_DOT: return (true);
	}
	return (false);
} 

static bool isoctprefix(int c) {
	return (c == 'o' || c == 'O');
}

static bool isoct(int c) {
	return (c >= '0' && c <= '7');
}

static bool ishexprefix(int c) {
	return (c == 'x' || c == 'X');
}

static bool ishex(int c) {
	return ((c >= '0' && c <= '9') ||
			(c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z'));
}

static int hex2dec(int c) {
	if (isdigit(c)) { return (c - '0'); }
	else if (islower(c)) { return ((c - 'a') + 10); }
	else if (isupper(c)) { return ((c - 'A') + 10); }
	return (-1);
}

static int read_oct(void) {
	if (!isoct(peek()))
		{ /* ERROR */ }
	int oct_num = get_c() - '0';
	if (isoct(peek())) { oct_num = (get_c() - '0') + (oct_num * 8); }
	return (oct_num);
}

static int read_hex(void) {
	if (!ishex(peek()))
		{ /* ERROR */ }
	int hex_num = hex2dec(get_c());
	if (ishex(peek()))
		{ hex_num = hex2dec(get_c()) + (hex_num * 16); }
	return (hex_num);
}

static int get_c(void) {
	int input_c;
	if (*parse_stream == EOS)
		{ return (EOS); }
	else if (match(REG_ESCAPE)) {
		input_c = *parse_stream++;
	
		if (isoctprefix(input_c))
			{ return (read_oct()); }
		else if (ishexprefix(input_c))
			{ return (read_hex()); }

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

static void expected(char expect_c) {
	if (!match(expect_c))
		{ /* ERROR */ }
}

static int str2int(void) {
	int digit = 0;
	while (isdigit(peek()))
		{ digit = (digit + (get_c() - '0')) + (digit * 10); }
	return (digit);
}

static node_ast_t* diff_range(node_ast_t* root) {
	if (root->alone != MULTI_S)
		{ /* ERROR */ }
	expected(REG_RBRACE);
	if (!match(REG_LBRACK))
		{ /* ERROR */ }
	node_ast_t* diff = reg_range();
	if (diff->alone != MULTI_S)
		{ /* ERROR */ }
	set_t* it = diff->cclass;
	while (it) {
		del_item_set(&root->cclass, it->item);
		it = it->next;
	}
	return (root);
}

static node_ast_t* bound_name(node_ast_t* root) {
	node_ast_t* rep_node = NULL;
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
	else
		{ /* ERROR */ }
	return (rep_node);
}

static node_ast_t* finite_seq(node_ast_t* root) {
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

static node_ast_t* curly_regex(node_ast_t* root) {
	node_ast_t* rep_node = NULL;
	if (match(REG_HYPHEN))
		{ return (diff_range(root));  }
	else if (!isdigit(peek()))
		{ rep_node = bound_name(root); }
	else
		{ rep_node = finite_seq(root); }
	expected(REG_RBRACE);
	return (rep_node);
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
			case REG_LBRACE: root = curly_regex(root);
				break;
		}
	}
	return (root);
}

static node_ast_t* reg_atom(void) {
	if (match(REG_LPAREN)) { 
		node_ast_t* expr = reg_expr();
		expected(REG_RPAREN);
		return (expr);
	}
	else if (match(REG_LBRACK)) { return (reg_range()); }
	else if (match(REG_DOT)) { return (dot_regex()); }
	else if (match(REG_DQUOTE)) { return (reg_quote()); }
	else if (!is_metachar(peek()))
		{ return (node_ast(AST_SYMBOL, ALONE_S, get_c())); }
	return (node_ast(AST_SYMBOL, ALONE_S, EPSILON));
}

static node_ast_t* reg_quote(void) {
	node_ast_t* root_concat = NULL;
	while (peek() != REG_DQUOTE && !is_end()) {
		int c = get_c();
		node_ast_t* symbol = node_ast(AST_SYMBOL, ALONE_S, c);
		if (!root_concat)
			{ root_concat = symbol; }
		else
			{ root_concat = node_ast(AST_CONCAT, root_concat, symbol); }
	}
	if (!root_concat)
		{ root_concat = node_ast(AST_SYMBOL, ALONE_S, EPSILON); }
	expected(REG_DQUOTE);
	return (root_concat);
}

static node_ast_t* reg_range(void) {
	bool negate = false;
	set_t* range = EMPTY_SET;
	if (match(REG_CARET))
		{ negate = true; }
	int last = 0;
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
	expected(REG_RBRACK);
	return (node_ast(AST_SYMBOL, MULTI_S, range));
}
