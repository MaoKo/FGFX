#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

#include "gen_dfa.h"
#include "gen.h"
#include "utils.h"
#include "lexical_spec.h"
#include "nfa.h"

void
gen_state_enum(int filde, lexical_spec_t const* spec) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	for (size_t i = 0; i < SIZE_VECTOR(spec->state_vect); ++i) {
		spec_entry_t* crt_state = (spec_entry_t*)AT_VECTOR(spec->state_vect, i);
		ENUM_STATE_LINE(filde, crt_state->name);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);

	spec_entry_t* init_state = AT_VECTOR(spec->state_vect, spec->start_state);
	dprintf(filde, DEFINE(%s, %s%s%s) NL NL,
						"INITIAL_STATE", STATE_PREFIX, SEP, init_state->name);
}

void
gen_token_enum(int filde, lexical_spec_t const* spec) {
	dprintf(filde, ENUM SP BEG_BLOCK NL);
	size_t count = 0;
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		if (!entry->is_frag) {
			ENUM_TOKEN_LINE(filde, entry->name);
			++count;
		}
	}

	ENUM_TOKEN_LINE(filde, "ERROR");
	ENUM_TOKEN_LINE(filde, "EOF");

	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL, MACRO_TOKEN, count + 2);
}

static inline void
gen_state_name(int filde, spec_entry_t const* entry) {
	char const* sep = ((entry) ? SEP : "");
	char const* name = ((entry) ? entry->name : "");
	dprintf(filde, "%s%s", name, sep);
}

static inline void
gen_typedef_name(int filde, char const* type_name, spec_entry_t const* entry) {
	gen_state_name(filde, entry);
	dprintf(filde, "%s_t", type_name);
}

void
gen_dfa_typedef(int filde, vector_t const* trans,
							vector_t const* final, spec_entry_t const* entry) {

	size_t size_trans = SIZE_VECTOR(trans);
	size_t size_final = SIZE_VECTOR(final);

	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB, min_size_type(size_trans));
	gen_typedef_name(filde, "dfa_state", entry);
	dprintf(filde, SEMI NL);

	dprintf(filde, TYPEDEF TAB "uint%u_t" TAB, min_size_type(size_final));
	gen_typedef_name(filde, "final_state", entry);
	dprintf(filde, SEMI NL NL);
}

void
gen_state_table(int filde, vector_t const* trans,
							char const* header, spec_entry_t const* entry) {
	dprintf(filde, STATIC SP);
	gen_typedef_name(filde, "dfa_state", entry);
	dprintf(filde, NL);

	gen_verbatim_file(filde, header);
	dprintf(filde, SEP);

	gen_state_name(filde, entry);
	dprintf(filde, "state_table[%zu][%d] = " BEG_BLOCK NL,
								SIZE_VECTOR(trans), MAX_ASCII);

	for (size_t i = 0; i < SIZE_VECTOR(trans); ++i) {
		trans_list_t const* list = (trans_list_t*)AT_VECTOR(trans, i);
		dprintf(filde, "/* %3zu */" TAB BEG_BLOCK, i);
		while (list) {
			trans_list_t const* next = list->next;
			trans_list_t const* min_range = contiguous_range(list);
			if (min_range != list) {
				dprintf(filde, "[%u ... %u]=%u",
							min_range->input, list->input, list->state);
				next = min_range->next;
			}
			else
				{ dprintf(filde, "[%u]=%u", list->input, list->state); }
			if (next)
				{ dprintf(filde, COMMA SP); }
			list = next;
		}
		dprintf(filde, END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_final_table(int filde, vector_t const* final,
							char const* header, spec_entry_t const* entry) {

	size_t count_final = SIZE_VECTOR(final) / 2;
	dprintf(filde, DEFINE(SIZE_FINAL_TAB, %zu) NL NL, count_final);

	dprintf(filde, STATIC SP);
	gen_typedef_name(filde, "final_state", entry);
	dprintf(filde, NL);

	gen_verbatim_file(filde, header);
	dprintf(filde, SEP);

	gen_state_name(filde, entry);
	dprintf(filde, "final_table[%s][2] = " BEG_BLOCK NL, "SIZE_FINAL_TAB");

	for (size_t i = 0; i < count_final; ++i) {
		dprintf(filde,	TAB BEG_BLOCK SP "%ld" COMMA SP
						TAB TOKEN_PREFIX SEP "%s" SP END_BLOCK COMMA NL,
						(long)AT_VECTOR(final, i*2),
						(char const*)AT_VECTOR(final, i*2+1));
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_skip_table(int filde, lexical_spec_t const* spec, char const* header) {
	vector_t* skip_table = new_vector();
	if (!skip_table)
		{ return; }

	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		if (entry->skip && (entry->kind == T_TERMINAL && !entry->is_frag))
			{ PUSH_BACK_VECTOR(skip_table, entry); }
	}
	
	if (SIZE_VECTOR(skip_table)) {
		dprintf(filde, STATIC SP INT NL);
		gen_verbatim_file(filde, header);

		dprintf(filde, "_skip_table[%zu] = " BEG_BLOCK NL,
					SIZE_VECTOR(skip_table) + 1);

		for (size_t i = 0; i < SIZE_VECTOR(skip_table); ++i) {
			spec_entry_t* entry = (spec_entry_t*)
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
