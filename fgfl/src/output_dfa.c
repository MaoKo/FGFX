#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

#include "output_dfa.h"
#include "output.h"
#include "utils.h"
#include "nfa.h"

static inline void
output_dfa_typedef(int filde, size_t size_trans, size_t size_final) {
	dprintf(filde, INCLUDE_SYS(stdint.h)"\n");
	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB "dfa_state_t;\n",
			min_size_type(size_trans));
	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB "final_state_t;\n\n",
			min_size_type(size_final));
}

static void
output_token_enum(int filde, vector_t const* elst) {
	dprintf(filde, ENUM" "BEG_BLOCK"\n");
	size_t count = 0;
	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(elst, i);
		if (entry->kind == GLOBAL || entry->kind == KEYWORD) {
			dprintf(filde, TAB TOKEN_PREFIX"%s,\n", entry->name);
			++count;
		}
	}
	dprintf(filde, TAB TOKEN_PREFIX"ERROR,\n");
	dprintf(filde, TAB TOKEN_PREFIX"EOF,\n");

	dprintf(filde, END_BLOCK""SEMI"\n\n");
	dprintf(filde, DEFINE(%s, %zu)"\n\n", MACRO_TOKEN, count + 2);
}

void
output_state_table(int filde, vector_t const* trans, char const* header) {
	dprintf(filde, STATIC" dfa_state_t\n");
	output_verbatim_file(filde, header);
	dprintf(filde, "_state_table[%zu][%d] = {\n",
			SIZE_VECTOR(trans), MAX_ASCII);

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

void
output_final_table(int filde, vector_t const* final, char const* header) {
	dprintf(filde, STATIC" final_state_t\n");
	output_verbatim_file(filde, header);
	dprintf(filde, "_final_table[][2] = {\n");

	size_t count_final = SIZE_VECTOR(final) / 2;
	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde, TAB"{ %ld, "TAB TOKEN_PREFIX"%s },\n",
				(long)AT_VECTOR(final, i*2),
				(char const*)AT_VECTOR(final, i*2+1));
	}
	dprintf(filde, "};\n\n"DEFINE(SIZE_FINAL_TAB, %zu)"\n\n", count_final);
}

void
output_skip_table(int filde, vector_t const* elst, char const* header) {
	vector_t* skip_table = new_vector();
	if (!skip_table)
		{ return; }

	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(elst, i);
		if (entry->skip && (entry->kind == GLOBAL))
			{ PUSH_BACK_VECTOR(skip_table, entry); }
	}
	
	if (SIZE_VECTOR(skip_table)) {
		dprintf(filde, STATIC" "INT"\n");
		output_verbatim_file(filde, header);
		dprintf(filde, "_skip_table[] = {\n");
		for (size_t i = 0; i < SIZE_VECTOR(skip_table); ++i) {
			token_entry_t* entry = (token_entry_t*)
					AT_VECTOR(skip_table, i);
			dprintf(filde, TAB TOKEN_PREFIX"%s,\n", entry->name);
		}
		dprintf(filde, TAB"-1,\n};\n\n");
	}
	else
		{ dprintf(filde, DEFINE(SKIP_TABLE_NOT_DEFINE,)"\n\n"); }
	del_vector(skip_table);
}

static inline void
output_dfa_useful_macro(int filde) {
	dprintf(filde, DEFINE(START_STATE, 1)"\n");
	dprintf(filde, DEFINE(DEAD_STATE, 0)"\n\n");
}

int
output_dfa_matrix(char const* base_file, vector_t const* trans,
		vector_t const* final, vector_t const* elst) {
	
	char const* header = strjoin(base_file, ".h");
	int filde = open_new_file(header);
	if (filde == -1)
		{ return (ERROR); }

	output_require_macro(filde, base_file);
	output_dfa_typedef(filde, SIZE_VECTOR(trans), SIZE_VECTOR(final));
	output_token_enum(filde, elst);
	IFNDEF_ONLY_TOKEN(filde);
	output_dfa_useful_macro(filde);
	output_state_table(filde, trans, header);
	output_final_table(filde, final, header);
	output_skip_table(filde, elst, header);
	ENDIF_ONLY_TOKEN(filde);
	output_endif(filde, base_file);

	if (close(filde) == -1)
		{ return (ERROR); }

	FREE(header);
	return (DONE);
}
