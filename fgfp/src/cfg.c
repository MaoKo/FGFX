#include <string.h>

#include "cfg.h"
#include "lexer.h"
#include "utils.h"

#define ONLY_TOKEN
#include "fgfx.lex.h"
#undef ONLY_TOKEN

static lexer_t* lex = NULL;

cfg_t*
new_cfg(void) {
	cfg_t* cfg = NEW(cfg_t, 1);
	if (!cfg)
		{ return (NULL); }
	memset(cfg, 0, sizeof(cfg_t));
	return (cfg);
}

void
del_cfg(cfg_t* cfg) {
	if (cfg) {
		for (size_t i = 0; i < HASH_SIZE; ++i) {
			del_vector(cfg->non_terminal[i]);
			del_vector(cfg->terminal[i]);
		}
		del_vector(cfg->productions);
	}
	FREE(cfg);
}

void
add_symbol_cfg(cfg_t* cfg, int kind) {
	size_t offset = 0;
	if (kind == TNON_TER)
		{ offset = 1; }
	char* name = strdup(BODY_BUFFER(LAST_LEXEME(lex)) + offset);
	if (kind == TNON_TER)
		{ *strchr(name, '>') = EOS; }
	vector_t** dst = (kind == TNON_TER) ? cfg->non_terminal : cfg->terminal;
	size_t hs = hash_str(name) % HASH_SIZE;
	int index = get_index_vector(dst[hs], name, &strcmp);
	if (index == -1) {
		if (!dst[hs])
			{ dst[hs] = new_vector(); }
		PUSH_BACK_VECTOR(dst[hs], name);
	}
	else
		{ FREE(name); }
}

static int cfg_syntax(cfg_t*);

cfg_t*
parse_cfg(int filde) {
	if (lex) {
		del_lexer(lex);
		lex = NULL;
	}
	if (!(lex = new_lexer(filde)))
		{ return (NULL); }
	cfg_t* cfg = new_cfg();
	if (!cfg)
		{ return (NULL); }
	if (cfg_syntax(cfg) == ERROR) {
		del_cfg(cfg);
		/* ERROR */
		return (NULL);
	}
	return (cfg);
}

static int cfg_prod(cfg_t*);
static int cfg_atom(cfg_t*);

int
cfg_syntax(cfg_t* cfg) {
	if (cfg_prod(cfg) == ERROR)
		{ return (ERROR); }
	while (peek_token(lex) != TEOF) {
		if (cfg_prod(cfg) == ERROR)
			{ return (ERROR); }
	}
	return (DONE);
}

int cfg_prod(cfg_t* cfg) {
	if (advance_token(lex) != TNON_TER) {
		/* ERROR */
		return (ERROR);
	}
	add_symbol_cfg(cfg, TNON_TER);
	if (advance_token(lex) != TARROW) {
		/* ERROR */
		return (ERROR);
	}
	if (cfg_atom(cfg) == ERROR)
		{ return (ERROR); }
	if (advance_token(lex) != TSEMI) {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}

int
cfg_atom(cfg_t* cfg) {
	if (in_first(lex, TNON_TER, TTOKEN, -1))
		{ add_symbol_cfg(cfg, advance_token(lex));  }
	else {
		/* ERROR */
		return (ERROR);
	}
	return (DONE);
}
