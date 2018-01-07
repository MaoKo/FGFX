#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexical_spec.h"
#include "preprocess_regex.h"
#include "regex.h"
#include "error.h"
#include "utils.h"

static inline bool
is_letter(char c) {
	return (isalpha(c) || c == '_');
}

size_t
size_ident(char const* str) {
	size_t size = 0;
	while (*str && (is_letter(*str) || isdigit(*str)))
		{ ++size; ++str; }
	return (size);
}

static int
find_macro_name(vector_t* src_vect, char const* beg_macro, size_t size) {
	for (size_t i = 0; i < SIZE_VECTOR(src_vect); ++i) {
		spec_entry_t* crt_entry = (spec_entry_t*)AT_VECTOR(src_vect, i);
		if (!strncmp(crt_entry->name, beg_macro, size))
			{ return (i); }
	}
	return (-1);
}

static int
dependency_macro(lexical_spec_t* spec, spec_entry_t* entry,
												bitset_t** set_macro) {
	if (!set_macro)
		{ return (ERROR); }
	else if (entry->kind == T_KEYWORD)
		{ return (DONE); }

	char const* beg_macro = strchr(entry->reg_str, '{');
	while (beg_macro) {
		if ((beg_macro == entry->reg_str) || (beg_macro[-1] != '\\')) {
			++beg_macro;

			if (is_letter(*beg_macro)) {
				bool find_dependence = false;
				int depend_not_token = -1;

				size_t size = size_ident(beg_macro);
				int index = find_macro_name(spec->entry_vect, beg_macro, size);

				if (index != -1) {
					find_dependence = true;
					spec_entry_t* crt_entry = (spec_entry_t*)
										AT_VECTOR(spec->entry_vect, index);

					if ((crt_entry->kind == T_KEYWORD))
						{ depend_not_token = T_KEYWORD; }
					else {
						if (!*set_macro)
							{ *set_macro = new_bitset(); }
						ADD_BITSET(*set_macro, GET_INDEX(crt_entry));
					}
				}
				else {
					int index = find_macro_name(spec->state_vect,
														beg_macro, size);
					if (index != -1) {
						find_dependence = true;
						depend_not_token = T_STATE;
					}
				}

				if (!find_dependence || (depend_not_token != -1)) {
					if (!find_dependence) {
						errorf(0, "Macro name %.*s is not defined anywhere.",
										size, beg_macro);
					}
					else if (depend_not_token != -1) {
						errorf(0, "Macro name %.*s is a %s.", size, beg_macro,
											(depend_not_token == T_KEYWORD)
													? "keyword" : "state" );
					}
					del_bitset(*set_macro);
					return (ERROR);
				}
			}
		}
		beg_macro = strchr(beg_macro + 1, '{');
	}
	return (DONE);
}

static int
recur_node_topsort(lexical_spec_t* spec, spec_entry_t* crt_entry,
			size_t index_entry, vector_t* stack_order, bitset_t* seen_reg) {

	crt_entry->is_used = true;
	if (IS_PRESENT(seen_reg, GET_INDEX(crt_entry))) {
		if (GET_INDEX(crt_entry) == index_entry) {
			errorf(0, "Stack overflow. Cycle detected for regex %s.",
											crt_entry->name);
			return (ERROR);
		}
		return (DONE);
	}
	ADD_BITSET(seen_reg, GET_INDEX(crt_entry));

	bitset_t* depend_node = NULL;
	if (dependency_macro(spec, crt_entry, &depend_node) == ERROR)
		{ return (ERROR); }

	int i;
	int exit_st = DONE;

	while ((i = IT_NEXT(depend_node)) != IT_NULL) {
		spec_entry_t* new_entry = (spec_entry_t*)
										AT_VECTOR(spec->entry_vect, i);
		if (recur_node_topsort(spec, new_entry, index_entry,
										stack_order, seen_reg) == ERROR)
			{ exit_st = ERROR; }
	}

	PUSH_BACK_VECTOR(stack_order, (void*)GET_INDEX(crt_entry));
	del_bitset(depend_node);

	return (exit_st);
}

vector_t*
topological_sort(lexical_spec_t* spec) {
	vector_t* stack_order = new_vector();
	bitset_t* seen_reg = new_bitset();

	int exit_st = DONE;
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_vect); ++i) {
		spec_entry_t* crt_entry = (spec_entry_t*)
										AT_VECTOR(spec->entry_vect, i);
		if (!((crt_entry->kind == T_TERMINAL) && (!crt_entry->is_frag)))
			{ continue; }
		else if (IS_PRESENT(seen_reg, GET_INDEX(crt_entry)))
			{ continue; }

		if (recur_node_topsort(spec, crt_entry, i,
										stack_order, seen_reg) == ERROR)
			{ exit_st = ERROR; }
	}

	if (exit_st == ERROR) {
		del_vector(stack_order);
		stack_order = NULL_VECT;
	}

	del_bitset(seen_reg);
	return (stack_order);
}

int
build_regex(lexical_spec_t* spec) {
	vector_t* stack_order;
	int exit_st = DONE;

	if ((stack_order = topological_sort(spec))) {
		for (size_t i = 0; i < SIZE_VECTOR(stack_order); ++i) {
			size_t j = (long)AT_VECTOR(stack_order, i);
			spec_entry_t* crt_entry = (spec_entry_t*)
											AT_VECTOR(spec->entry_vect, j);
			if (crt_entry->kind != T_KEYWORD)
				{ crt_entry->reg = regex_to_ast(spec, crt_entry->reg_str); }
		}
	}
	else
		{ exit_st = ERROR; }

	del_vector(stack_order);
	return (exit_st);
}

