#include "tokendef.h"

static void skip_space(char** line) {
	while (**line && (**line == ' ' || **line == '\t'))
		{ ++(*line); }
}

static int is_letter(int c) {
	return (isalpha(c) || c == '_');
}

static char* strjoin(char const* str1, char const* str2) {
	size_t const s1 = strlen(str1);
	size_t const s2 = strlen(str2);
	char* frame = NEW(char, s1 + s2 + 1);
	if (!frame)
		{ return (NULL); }
	memcpy(frame, str1, sizeof(char) * s1);
	memcpy(frame + s1, str2, sizeof(char) * s2);
	frame[s1 + s2] = EOS;
	return (frame);
}

static char* get_token_name(char** line) {
	if (!is_letter(**line))
		{ return (NULL); }
	char* ident = strdup(*line);
	char* end = ident;
	while (is_letter(*end) || isdigit(*end))
		{ ++end; }
	*end = EOS;
	*line += (end - ident);
	return (ident);
}

static size_t extend_line(char** line, size_t index, size_t crt_size) {
	if (index + 1 >= crt_size) {
		size_t new_sz = round_up(crt_size);
		if (!(*line = REALLOC(*line, new_sz)))
			{ return (0); }
		crt_size = new_sz;
	}
	return (crt_size);
}

static int get_next_line(int filde, char** line) {
	size_t crt_size = BUFF_SIZE;
	if (!line || !(*line = NEW(char, crt_size)))
		{ return (ERROR); }
	size_t index = 0;
	int end_file = NO_READ;
	while (1) {
		if (!(crt_size = extend_line(line, index, crt_size)))
			{ return (ERROR); }
		int rd;
		if ((rd = read(filde, (*line) + index, 1)) == -1)
			{ return (ERROR); }
		else if (!rd) {
			if (rd == NO_READ) {
				FREE(*line);
				*line = NULL;
			}
			return (end_file); 
		}
		else if ((*line)[index++] == '\n') {
			(*line)[--index] = '\0';
			return (READ);
		}
		end_file = READ;
	}
}

static inline bool is_not_terminated(char* line) {
	if (!*line)
		{ return (false); }
	while (line[1] != EOS)
		{ ++line; }
	return (*line == '\\');
}

static char* read_multi_line(int filde) {
	char* line = NULL;
	get_next_line(filde, &line);
	while (line && is_not_terminated(line)) {
		char* follow = NULL;
		get_next_line(filde, &follow);
		skip_space(&follow);
		*strrchr(line, '\\') = EOS;
		line = strjoin(line, follow);
	}
	return (line);
}

static int get_token_entry(int filde, token_entry_t** te, vector_t const* symbol) {
	char* line = read_multi_line(filde);
	if (!line)
		{ return (NO_READ); }
	skip_space(&line);
	if (*line == ENTRY_COM_SYM || *line == EOS)
		{ return (READ); }
	*te = NEW(token_entry_t, 1);
	if (!*te)
		{ return (ERROR); }
	bool local = false;
	if (*line == ENTRY_LOCAL_SYM) {
		++line;
		local = true;
	}
	(*te)->local = local;
	(*te)->name = get_token_name(&line);
	skip_space(&line);
	if (*line++ != ENTRY_EQUAL_SYM)
		{ return (ERROR); }
	skip_space(&line);
	(*te)->reg = regex2ast(line, symbol);
	return (READ);
}

static void del_token_entry(token_entry_t* te) {
	if (te) {	
		del_node_ast(te->reg);
		//del fragment te->frag
		FREE(te);
	}
}

void del_token_spec(token_spec_t* spec) {
	if (spec) {
		for (size_t i = 0; i < size_vector(spec->entry_lst); ++i)
			{ del_token_entry(at_vector(spec->entry_lst, i)); }
		del_vector(spec->entry_lst);
		FREE(spec);
	}
}

int retreive_token_entry(char const* pathname, token_spec_t** spec) {
	if (!spec)
		{ return (ERROR); }
	*spec = NEW(token_spec_t, 1);
	if (!*spec || (((*spec)->filde = open(pathname, O_RDONLY)) == -1))
		{ return (ERROR); }
	(*spec)->entry_lst = vector();
	token_entry_t* entry = NULL;
	while (get_token_entry((*spec)->filde, &entry, (*spec)->entry_lst) == READ) {
		if (entry)
			{ push_back_vector((*spec)->entry_lst, (void*)entry); }
		entry = NULL;
	}
	return (READ);
}

int token_spec2nfa(token_spec_t* spec) {
	if (!spec)
		{ return (ERROR); }
	set_t* master = EMPTY_SET;
	for (size_t i = 0; i < size_vector(spec->entry_lst); ++i) {
		token_entry_t* te = (token_entry_t*)at_vector(spec->entry_lst, i);
		if (!te->local) {
			te->frag = reg_ast2nfa(te->reg, i + 1);
			add_set(&master, te->frag);
			del_node_ast(te->reg); te->reg = NULL;
		}
	}
	spec->master = merge_nfa_fragment(master);
	return (READ);
}
