#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

#include "gen_dfa.h"
#include "gen.h"
#include "utils.h"
#include "token_spec.h"
#include "nfa.h"

void
gen_dfa_typedef(int filde, size_t size_trans, size_t size_final) {
	dprintf(filde, INCLUDE_SYS(stdint.h) NL NL);
	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB "dfa_state_t" SEMI NL,
			min_size_type(size_trans));
	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB "final_state_t" SEMI NL NL,
			min_size_type(size_final));
}

void
gen_token_enum(int filde, vector_t const* elst) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	size_t count = 0;
	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(elst, i);
		if (entry->kind == GLOBAL || entry->kind == KEYWORD) {
			ENUM_LINE(filde, entry->name);
			++count;
		}
	}

	ENUM_LINE(filde, "ERROR");
	ENUM_LINE(filde, "EOF");

	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL, MACRO_TOKEN, count + 2);
}

void
gen_state_table(int filde, vector_t const* trans, char const* header) {
	dprintf(filde, STATIC SP "dfa_state_t" NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, "_state_table[%zu][%d] = " BEG_BLOCK NL,
			SIZE_VECTOR(trans), MAX_ASCII);

	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i) {
		trans_list_t const* t = (trans_list_t*)AT_VECTOR(trans, i);
		dprintf(filde, "/* %3zu */" TAB BEG_BLOCK, i);
		while (t) {
			trans_list_t const* next = t->next;
			trans_list_t const* min_range = contiguous_range(t);
			if (min_range != t) {
				dprintf(filde, "[%u ... %u]=%u",
					min_range->input, t->input, t->state);
				next = min_range->next;
			}
			else {
				dprintf(filde, "[%u]=%u",
					t->input, t->state);
			}
			if (next)
				{ dprintf(filde, COMMA SP); }
			t = next;
		}
		dprintf(filde, END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_final_table(int filde, vector_t const* final, char const* header) {
	size_t count_final = SIZE_VECTOR(final) / 2;
	dprintf(filde, DEFINE(SIZE_FINAL_TAB, %zu) NL NL, count_final);

	dprintf(filde, STATIC SP "final_state_t" NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, "_final_table[%s][2] = " BEG_BLOCK NL, "SIZE_FINAL_TAB");
	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde, TAB BEG_BLOCK SP "%ld" COMMA SP
						TAB TOKEN_PREFIX SEP "%s" SP END_BLOCK COMMA NL,
						(long)AT_VECTOR(final, i*2),
						(char const*)AT_VECTOR(final, i*2+1));
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_skip_table(int filde, vector_t const* elst, char const* header) {
	vector_t* skip_table = new_vector();
	if (!skip_table)
		{ return; }

	for (size_t i = 0; i < SIZE_VECTOR(elst); ++i) {
		token_entry_t* entry = (token_entry_t*)AT_VECTOR(elst, i);
		if (entry->skip && (entry->kind == GLOBAL))
			{ PUSH_BACK_VECTOR(skip_table, entry); }
	}
	
	if (SIZE_VECTOR(skip_table)) {
		dprintf(filde, STATIC SP INT NL);
		gen_verbatim_file(filde, header);

		dprintf(filde, "_skip_table[%zu] = " BEG_BLOCK NL,
					SIZE_VECTOR(skip_table) + 1);

		for (size_t i = 0; i < SIZE_VECTOR(skip_table); ++i) {
			token_entry_t* entry = (token_entry_t*)
					AT_VECTOR(skip_table, i);
			dprintf(filde, TAB TOKEN_PREFIX SEP "%s" COMMA NL, entry->name);
		}
		dprintf(filde, TAB "-1" COMMA NL END_BLOCK SEMI NL NL);
	}
	else
		{ dprintf(filde, DEFINE(SKIP_TABLE_NOT_DEFINE,) NL NL); }
	del_vector(skip_table);
}

void
gen_dfa_useful_macro(int filde) {
	dprintf(filde, DEFINE(START_STATE, 1) NL);
	dprintf(filde, DEFINE(DEAD_STATE, 0) NL NL);
}
