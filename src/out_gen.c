#include "out_gen.h"

static size_t
min_size_type(size_t size) {
	if (size <= UCHAR_MAX) { return (8); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (16); }
	return (32);
}

static inline void
out_require_header(int filde) {
	write_str_fd(filde, INCLUDE_SYS(stdint.h)"\n\n");
}

static void
out_enum(int filde, vector_t const* elst) {
	write_str_fd(filde, ENUM" "BEG_BLOCK"\n");
	write_str_fd(filde, TAB"TNONE,\n");
	size_t count = 0;
	for (size_t i = 0; i < size_vector(elst); ++i) {
		token_entry_t* te = (token_entry_t*)at_vector(elst, i);
		if (!te->local) {
			dprintf(filde, TAB"T%s,\n", te->name);
			++count;
		}
	}
	write_str_fd(filde, TAB"TEOF,\n");
	write_str_fd(filde, END_BLOCK""SEMI"\n\n");
	dprintf(filde, "#define TOTAL_TOKEN"TAB"%zu\n\n", count + 2);
}

static void
out_state_table(int filde, vector_t const* trans) {
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

static void
out_final_table(int filde, vector_t const* final) {
	dprintf(filde, STATIC" uint%zu_t final_table[][2] = {\n",
			min_size_type(size_vector(final)));
	size_t count_final = size_vector(final) / 2;
	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde, TAB"{ %ld, "TAB"T%s },\n",
				(long)at_vector(final, i*2),
				(char const*)at_vector(final, i*2+1));
	}
	dprintf(filde, "};\n\n#define SIZE_FINAL_TAB"TAB"%zu\n\n", count_final);
}

static void
out_useful_macro(int filde) {
	write_str_fd(filde, DEFINE(START_STATE, 1)"\n");
	write_str_fd(filde, DEFINE(DEAD_STATE, 0)"\n\n");
}

static void
out_include_macro(int filde, char const* header) {
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

static void
out_require_macro(int filde, char const* header) {
	write_str_fd(filde, "#ifndef ");
	out_include_macro(filde, header);
	write_str_fd(filde, "\n");
	write_str_fd(filde, "#define ");
	out_include_macro(filde, header);
	write_str_fd(filde, "\n\n");
}

static void
out_endif(int filde, char const* header) {
	write_str_fd(filde, "#endif /* ");
	out_include_macro(filde, header);
	write_str_fd(filde, " */");
}

int
out_header(char const* header, vector_t const* trans,
		vector_t const* final, vector_t const* elst) {
	int flag = O_CREAT;
	if (file_exist(header))
		{ flag = O_TRUNC; }
	int filde = open(header, O_WRONLY | flag, 0666);
	if (filde == -1)
		{ return (ERROR); }
	out_require_macro(filde, header);
	out_require_header(filde);
	out_enum(filde, elst);
	out_useful_macro(filde);
	out_state_table(filde, trans);
	out_final_table(filde, final);
	out_endif(filde, header);
	if (close(filde) == -1)
		{ return (ERROR); }
	return (0);
}
