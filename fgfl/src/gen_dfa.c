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
	size_t size_state = SIZE_VECTOR(spec->state_vect);
	dprintf(filde, ENUM SP BEG_BLOCK NL);

	for (size_t i = 0; i < size_state; ++i) {
		spec_entry_t* crt_state = (spec_entry_t*)AT_VECTOR(spec->state_vect, i);
		ENUM_STATE_LINE(filde, crt_state->name);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);

	spec_entry_t* init_state = AT_VECTOR(spec->state_vect, spec->start_state);

	dprintf(filde, DEFINE(%s, %zu) NL, MACRO_STATE, size_state);
	dprintf(filde, DEFINE(%s, %s%s%s) NL NL,
						"INIT_STATE", STATE_PREFIX, SEP, init_state->name);
}

void
gen_token_enum(int filde, lexical_spec_t const* spec) {
	size_t size_entry = SIZE_VECTOR(spec->entry_vect);
	dprintf(filde, ENUM SP BEG_BLOCK NL);

	for (size_t i = 0; i < size_entry; ++i) {
		spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		ENUM_TOKEN_LINE(filde, entry->name);
	}

	ENUM_TOKEN_LINE(filde, "ERROR");
	ENUM_TOKEN_LINE(filde, "EOF");

	dprintf(filde, END_BLOCK SEMI NL NL);
	dprintf(filde, DEFINE(%s, %zu) NL NL, MACRO_TOKEN, size_entry + 2);
}

static inline void
gen_state_name(int filde, spec_entry_t const* entry) {
	char const* sep = ((entry) ? SEP : "");
	char const* name = ((entry) ? entry->name : "");
	dprintf(filde, "%s%s", name, sep);
}

void
gen_state_table(int filde, lexical_spec_t const* spec,
							char const* header, spec_entry_t const* state) {
	size_t size_trans = SIZE_VECTOR(spec->trans);

	dprintf(filde, STATIC SP "uint%u_t" NL, min_size_type(size_trans, true));
	gen_verbatim_file(filde, header);

	dprintf(filde, SEP);
	gen_state_name(filde, state);

	dprintf(filde, "state_table[%zu][%d] = "
									BEG_BLOCK NL, size_trans, MAX_ASCII);

	for (size_t i = 0; i < size_trans; ++i) {
		trans_list_t const* list = (trans_list_t*)AT_VECTOR(spec->trans, i);
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
gen_middle_table(int filde, lexical_spec_t const* spec,
							char const* header, spec_entry_t const* state) {
	if (!spec->middle)
		{ return; }

	size_t size_middle = SIZE_VECTOR(spec->middle);

	dprintf(filde, STATIC SP "uint8_t" NL);
	gen_verbatim_file(filde, header);

	dprintf(filde, SEP);
	gen_state_name(filde, state);

	dprintf(filde, "middle_table[%zu] = " BEG_BLOCK NL,
											SIZE_VECTOR(spec->trans));

	for (size_t i = 0; i < size_middle; ++i) {
		long beg_state = (long)AT_VECTOR(spec->middle, i);
		dprintf(filde, TAB "[%ld]=true" COMMA NL, beg_state);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_final_table(int filde, lexical_spec_t const* spec,
							char const* header, spec_entry_t const* state) {
	size_t size_final = SIZE_VECTOR(spec->final);
	
	dprintf(filde, STATIC SP "uint%u_t" NL,
								min_size_type(SIZE_VECTOR(spec->trans), true));
	gen_verbatim_file(filde, header);

	dprintf(filde, SEP);
	gen_state_name(filde, state);

	size_t count_final = size_final / 2;
	dprintf(filde, "final_table[%zu][2] = " BEG_BLOCK NL, count_final + 1);

	for (size_t i = 0; i < count_final; ++i) {
		long final_state = (long)AT_VECTOR(spec->final, i*2);
		char const* token_name = (char const*)AT_VECTOR(spec->final, (i*2) + 1);

		dprintf(filde,	TAB BEG_BLOCK SP "%ld" COMMA SP
							TAB TOKEN_PREFIX SEP "%s" SP END_BLOCK COMMA NL,
							final_state, token_name);
	}
	dprintf(filde, TAB BEG_BLOCK SP "0" SP END_BLOCK COMMA NL);
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_change_state(int filde, char const* header, lexical_spec_t* spec) {
	size_t size_entry = SIZE_VECTOR(spec->entry_vect);
	dprintf(filde, STATIC SP "int%u_t" NL,
							min_size_type(size_entry, false));

	gen_verbatim_file(filde, header);
	dprintf(filde, "_begin_table[%s][%s] = "
								BEG_BLOCK NL, MACRO_TOKEN, MACRO_STATE);

	for (size_t i = 0; i < size_entry; ++i) {
		spec_entry_t* crt_entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		bool already_print = false;

		if ((crt_entry->kind == T_TERMINAL)) {
			trans_list_t* it_lst = crt_entry->state_begin_lst;
			while (it_lst) {
				if (it_lst->state != NONE_BEGIN) {
					if (!already_print) {
						dprintf(filde, TAB);
						already_print = true;
					}
				
					spec_entry_t* in_state = ((spec_entry_t*)AT_VECTOR(
									spec->state_vect, (size_t)it_lst->input));

					spec_entry_t* out_state = ((spec_entry_t*)AT_VECTOR(
									spec->state_vect, (size_t)it_lst->state));
		
					dprintf(filde, "[" TOKEN_PREFIX SEP "%s]", crt_entry->name);
					dprintf(filde, "[" STATE_PREFIX SEP "%s]", in_state->name);

					dprintf(filde, " = " STATE_PREFIX SEP "%s"
												COMMA SP, out_state->name);
				}
				it_lst = it_lst->next;
			}
			if (already_print)
				{ dprintf(filde, NL); }
		}
	}

//	dprintf(filde, TAB "-1" COMMA NL); // for T_ERROR
//	dprintf(filde, TAB "-1" COMMA NL); // for T_EOF

	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
gen_terminated_table(int filde, lexical_spec_t const* spec,
											char const* header, size_t kind) {
	vector_t* dst_vect = new_vector();
	if (!dst_vect)
		{ return; }

	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(spec->entry_vect, i);
		if ((entry->kind == T_TERMINAL) && (!entry->is_frag)) {
			if (kind == SKIP_TABLE && entry->skip)
				{ PUSH_BACK_VECTOR(dst_vect, entry); }
			else if (kind == LOOK_TABLE && entry->use_look)
				{ PUSH_BACK_VECTOR(dst_vect, entry); }
		}
	}

	size_t size_table = SIZE_VECTOR(dst_vect);
	if (size_table) {
		dprintf(filde, STATIC SP "int%u_t" NL,
								min_size_type(size_table, false));

		gen_verbatim_file(filde, header);
		dprintf(filde, "_%s_table[%zu] = " BEG_BLOCK NL,
				((kind == SKIP_TABLE) ? "skip" : "look"), size_table + 1);

		for (size_t i = 0; i < size_table; ++i) {
			spec_entry_t* entry = (spec_entry_t*)AT_VECTOR(dst_vect, i);
			dprintf(filde, TAB TOKEN_PREFIX SEP "%s" COMMA NL, entry->name);
		}
		dprintf(filde, TAB "-1" COMMA NL END_BLOCK SEMI NL NL);
	}
	else {
		dprintf(filde, DEFINE(%s_TABLE_NOT_DEFINE,) NL NL,
								((kind == SKIP_TABLE) ? "SKIP" : "LOOK"));
	}
	del_vector(dst_vect);
}
