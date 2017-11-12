#include "out_gen.h"

static size_t min_size_type(size_t size) {
	if (size <= UCHAR_MAX) { return (8); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (16); }
	return (32);
}

static void out_require_header(int filde) {
	write_str_fd(filde, INCLUDE_SYS(stdint.h)"\n");
	write_str_fd(filde, INCLUDE_SYS(stddef.h)"\n\n");
}

static void out_is_final_item(int filde) {
	write_str_fd(filde, INT" " FUNC(is_final_state, int state)" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB FOR(int i = 0, i < SIZE_FINAL_TAB, ++i)
			" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB TAB IF(final_table[i][0] == state)"\n");
	write_str_fd(filde, TAB TAB TAB BEG_BLOCK" "
			RETURN(final_table[i][1])""SEMI" "END_BLOCK"\n");
	write_str_fd(filde, TAB END_BLOCK"\n");
	write_str_fd(filde, TAB RETURN(TNONE)""SEMI"\n");
	write_str_fd(filde, END_BLOCK"\n\n");
}

static void out_get_next_token(int filde) {
	write_str_fd(filde, INT" " FUNC(get_next_token, char const* str, size_t* len)
			" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB IF(!str)"\n");
	write_str_fd(filde, TAB TAB BEG_BLOCK" "
			RETURN(TNONE)""SEMI" "END_BLOCK"\n");

	write_str_fd(filde, TAB"char const* base = str"SEMI"\n\n");

	write_str_fd(filde, TAB"int state = START_STATE"SEMI"\n");
	write_str_fd(filde, TAB"int last_tok = TNONE"SEMI"\n\n");

	write_str_fd(filde, TAB IF(len)"\n");
	write_str_fd(filde, TAB TAB BEG_BLOCK " *len = 0"SEMI" "END_BLOCK"\n\n");

	write_str_fd(filde, TAB WHILE(*str && (state != DEAD_STATE))
			" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB TAB"int index = *str"SEMI"\n");
	write_str_fd(filde, TAB TAB"state = state_table[state][index]"SEMI"\n");
	write_str_fd(filde, TAB TAB IF(is_final_state(state))"\n");
	write_str_fd(filde, TAB TAB TAB BEG_BLOCK
			" last_tok = is_final_state(state)"SEMI" "END_BLOCK"\n");

	write_str_fd(filde, TAB TAB IF(state != DEAD_STATE)"\n");
	write_str_fd(filde, TAB TAB TAB BEG_BLOCK" ++str"SEMI" "END_BLOCK"\n");
	write_str_fd(filde, TAB END_BLOCK"\n\n");

	write_str_fd(filde, TAB IF(!*str && last_tok == TNONE)"\n");
	write_str_fd(filde, TAB TAB BEG_BLOCK" "
			RETURN(TEOF)""SEMI" "END_BLOCK"\n\n");
	
	write_str_fd(filde, TAB IF(len)"\n");
	write_str_fd(filde, TAB TAB BEG_BLOCK " *len = str - base"
			SEMI" "END_BLOCK"\n\n");
	
	write_str_fd(filde, TAB RETURN(last_tok) SEMI"\n");
	write_str_fd(filde, END_BLOCK"\n");
}

static void out_enum(int filde, vector_t* elst) {
	write_str_fd(filde, ENUM" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB"TNONE,\n");
	for (size_t i = 0; i < size_vector(elst); ++i) {
		token_entry_t* te = (token_entry_t*)at_vector(elst, i);
		if (!te->local)
			{ dprintf(filde, TAB"T%s,\n", te->name); }
	}
	write_str_fd(filde, TAB"TEOF,\n");
	write_str_fd(filde, END_BLOCK""SEMI"\n\n");
	dprintf(filde, "#define TOTAL_TOKEN"TAB"%zu\n\n", size_vector(elst) + 2);
}

static void out_state_table(int filde, vector_t* trans) {
	dprintf(filde, "//Size of state table = %zu\n", size_vector(trans));
	dprintf(filde, STATIC" uint%zu_t state_table[][%d] = {\n",
			min_size_type(size_vector(trans)), MAX_ASCII);

	for (size_t i = 0; i < size_vector(trans); ++i) {
		vector_t* state = (vector_t*)at_vector(trans, i);
		write_str_fd(filde, TAB BEG_BLOCK);
		for (size_t j = 0; j < size_vector(state); ++j) {
			long out = (long)at_vector(state, j);
			if (out)
				{ dprintf(filde, "[%zu]=%ld, ", j, out); }
		}
		write_str_fd(filde, END_BLOCK",\n");
	}
	write_str_fd(filde, END_BLOCK""SEMI"\n\n");
}

static void out_final_table(int filde, vector_t* final) {
	dprintf(filde, STATIC" uint%zu_t final_table[][2] = {\n",
			min_size_type(size_vector(final)));
	size_t count_final = size_vector(final) / 2;
	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde, TAB"{%ld, "TAB"T%s},\n", (long)at_vector(final, i*2),
				(char const*)at_vector(final, i*2+1));
	}
	dprintf(filde, "};\n\n#define SIZE_FINAL_TAB"TAB"%zu\n\n", count_final);
}

static void out_useful_macro(int filde) {
	write_str_fd(filde, DEFINE(START_STATE, 1)"\n");
	write_str_fd(filde, DEFINE(DEAD_STATE, 0)"\n\n");
}

static void out_include_macro(int filde, char const* header) {
	while (*header) {
		char c;
		if (isalnum(*header))
			{ c = toupper(*header); }
		else
			{ c = '_'; }	
		write(filde, &c, 1);
		++header;
	}
}

static void out_require_macro(int filde, char const* header) {
	write_str_fd(filde, "#ifndef ");
	out_include_macro(filde, header);
	write_str_fd(filde, "\n");
	write_str_fd(filde, "#define ");
	out_include_macro(filde, header);
	write_str_fd(filde, "\n\n");
}

static void out_endif(int filde, char const* header) {
	write_str_fd(filde, "#endif /* ");
	out_include_macro(filde, header);
	write_str_fd(filde, " */");
}

static void out_prototype(int filde) {
	write_str_fd(filde, INT" "FUNC(is_state_final, int)SEMI"\n");
	write_str_fd(filde, INT" "
			FUNC(get_next_token, char const*, size_t*)SEMI"\n\n");
}

int out_header(char const* header, vector_t* elst) {
	int filde = open(header, O_WRONLY | O_CREAT, 0666);
	if (filde == -1)
		{ return (ERROR); }
	out_require_macro(filde, header);
	out_require_header(filde);
	out_enum(filde, elst);
	out_useful_macro(filde);
	out_prototype(filde);
	out_endif(filde, header);
	if (close(filde) == -1)
		{ return (ERROR); }
	return (0);
}

static void out_include_header(int filde, char const* header) {
	write_str_fd(filde, "#include \"");
	write_str_fd(filde, header);
	write_str_fd(filde, "\"\n\n");
}

int out_body(char const* body, char const* header,
		vector_t* trans, vector_t* final) {
	int filde = open(body, O_WRONLY | O_CREAT, 0666);
	if (filde == -1)
		{ return (ERROR); }
	out_include_header(filde, header);
	out_state_table(filde, trans);
	out_final_table(filde, final);
	out_is_final_item(filde);
	out_get_next_token(filde);
	if (close(filde) == -1)
		{ return (ERROR); }
	return (0);
}

