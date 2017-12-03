#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

#include "dfagen.h"
#include "utils.h"
#include "nfa.h"

static size_t
min_size_type(size_t size) {
	if (size <= UCHAR_MAX) { return (8); }
	else if (size > UCHAR_MAX && size <= USHRT_MAX)
		{ return (16); }
	return (32);
}

static inline void
gen_require_header(int filde, vector_t const* trans, vector_t const* final) {
	dprintf(filde, INCLUDE_SYS(stdint.h)"\n");
	dprintf(filde, TYPEDEF""TAB"uint%zu_t"TAB"dfa_state_t;\n",
			min_size_type(SIZE_VECTOR(trans)));
	dprintf(filde, TYPEDEF""TAB"uint%zu_t"TAB"final_state_t;\n\n",
			min_size_type(SIZE_VECTOR(final)));
}

static void
gen_enum(int filde, vector_t const* elst) {
	dprintf(filde, ENUM" "BEG_BLOCK"\n");
	dprintf(filde, TAB"TNONE,\n");
	size_t count = 0;
	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* te = (token_entry_t*)AT_VECTOR(elst, i);
		if (!te->local) {
			dprintf(filde, TAB"T%s,\n", te->name);
			++count;
		}
	}
	dprintf(filde, TAB"TEOF,\n");
	dprintf(filde, END_BLOCK""SEMI"\n\n");
	dprintf(filde, "#define TOTAL_TOKEN"TAB"%zu\n\n", count + 2);
}

static void print_verbatim_header(int filde, char const* header) {
	if (isdigit(*header))
		{ dprintf(filde, "_"); }
	while (*header && (isalnum(*header) || (*header == '_')))
		{ dprintf(filde, "%c", *(header++)); }
}

static void
gen_state_table(int filde, vector_t const* trans, char const* header) {
	dprintf(filde, "//Size of state table = %zu\n", SIZE_VECTOR(trans));
	dprintf(filde, STATIC" dfa_state_t ");
	print_verbatim_header(filde, header);
	dprintf(filde, "_state_table[][%d] = {\n", MAX_ASCII);

	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i) {
		trans_list_t const* t = (trans_list_t*)AT_VECTOR(trans, i);
		dprintf(filde, "/* %3zu */", i);
		dprintf(filde, TAB BEG_BLOCK);
		while (t) {
			trans_list_t const* next = t->next;
			trans_list_t const* min_range = contiguous_range(t);
			if (min_range != t) {
				dprintf(filde, "[%u ... %u]=%u",
					min_range->input, t->input, t->state);
				next = min_range->next;
			}
			else
				{ dprintf(filde, "[%u]=%u", t->input, t->state); }
			if (next)
				{ dprintf(filde, ", "); }
			t = next;
		}
		dprintf(filde, END_BLOCK",\n");
	}
	dprintf(filde, END_BLOCK""SEMI"\n\n");
}

static void
gen_final_table(int filde, vector_t const* final, char const* header) {
	dprintf(filde, STATIC" final_state_t ");
	print_verbatim_header(filde, header);
	dprintf(filde, "_final_table[][2] = {\n");

	size_t count_final = SIZE_VECTOR(final) / 2;
	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde, TAB"{ %ld, "TAB"T%s },\n",
				(long)AT_VECTOR(final, i*2),
				(char const*)AT_VECTOR(final, i*2+1));
	}
	dprintf(filde, "};\n\n#define SIZE_FINAL_TAB"TAB"%zu\n\n", count_final);
}

static void
gen_skip_table(int filde, vector_t const* elst, char const* header) {
	dprintf(filde, STATIC" "INT" ");
	print_verbatim_header(filde, header);
	dprintf(filde, "_skip_table[] = {\n");
	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(elst, i);
		if (entry->skip && !entry->local)
			{ dprintf(filde, TAB"T%s,\n", entry->name); }
	}
	dprintf(filde, TAB"-1,\n};\n\n");
}

static inline void
gen_useful_macro(int filde) {
	dprintf(filde, DEFINE(START_STATE, 1)"\n");
	dprintf(filde, DEFINE(DEAD_STATE, 0)"\n\n");
}

static void
gen_include_macro(int filde, char const* header) {
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
gen_require_macro(int filde, char const* header) {
	dprintf(filde, "#ifndef ");
	gen_include_macro(filde, header);
	dprintf(filde, "\n");
	dprintf(filde, "#define ");
	gen_include_macro(filde, header);
	dprintf(filde, "\n\n");
}

static void
gen_endif(int filde, char const* header) {
	dprintf(filde, "#endif /* ");
	gen_include_macro(filde, header);
	dprintf(filde, " */");
}

int
gen_header(char const* header, vector_t const* trans,
		vector_t const* final, vector_t const* elst) {
	int flag = O_CREAT;
	if (file_exist(header))
		{ flag = O_TRUNC; }
	int filde = open(header, O_WRONLY | flag, 0666);
	if (filde == -1)
		{ return (ERROR); }
	gen_require_macro(filde, header);
	gen_require_header(filde, trans, final);
	gen_enum(filde, elst);
	dprintf(filde, "#ifndef ONLY_TOKEN\n\n");
	gen_useful_macro(filde);
	gen_state_table(filde, trans, header);
	gen_final_table(filde, final, header);
	gen_skip_table(filde, elst, header);
	dprintf(filde, "#endif /* ONLY_TOKEN */\n");
	gen_endif(filde, header);
	if (close(filde) == -1)
		{ return (ERROR); }
	return (0);
}
