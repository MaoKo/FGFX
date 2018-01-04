#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "token_def.h"
#include "preprocess_regex.h"
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
dependency_macro(token_spec_t* spec, token_entry_t* entry,
												bitset_t** set_macro) {
	if (!set_macro)
		{ return (ERROR); }
	else if (entry->kind == KEYWORD)
		{ return (DONE); }

	char const* beg_macro = strchr(entry->reg_str, '{');
	while (beg_macro) {
		if ((beg_macro == entry->reg_str) || (beg_macro[-1] != '\\')) {
			++beg_macro;

			if (is_letter(*beg_macro)) {
				bool find_dependence = false;
				bool depend_keyword = false;

				size_t size = size_ident(beg_macro);

				for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
					token_entry_t* crt_entry = (token_entry_t*)
											AT_VECTOR(spec->entry_lst, i);

					if (!strncmp(crt_entry->name, beg_macro, size)) {
						find_dependence = true;
						if (crt_entry->kind == KEYWORD)
							{ depend_keyword = true; }
						else {
							if (!*set_macro)
								{ *set_macro = new_bitset(); }
							ADD_BITSET(*set_macro, crt_entry->index);
						}
					}
				}
				if (!find_dependence || depend_keyword) {
					if (!find_dependence) {
						errorf(0, "Macro name %s not defined anywhere.",
										beg_macro);
					}
					else if (depend_keyword) {
						errorf(0, "Macro name %s is a keyword.",
										beg_macro);
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
recur_node_topsort(token_spec_t* spec, token_entry_t* crt_entry,
			size_t index_entry, vector_t* stack_order, bitset_t* seen_reg) {

	if (IS_PRESENT(seen_reg, crt_entry->index)) {
		if (crt_entry->index == index_entry) {
			errorf(0, "Stack overflow. Cycle detected for regex %s.",
											crt_entry->name);
			return (ERROR);
		}
		return (DONE);
	}
	ADD_BITSET(seen_reg, crt_entry->index);

	bitset_t* depend_node = NULL;
	if (dependency_macro(spec, crt_entry, &depend_node) == ERROR)
		{ return (ERROR); }

	int i;
	int exit_st = DONE;

	while ((i = IT_NEXT(depend_node)) != IT_NULL) {
		token_entry_t* new_entry = (token_entry_t*)
										AT_VECTOR(spec->entry_lst, i);
		if (recur_node_topsort(spec, new_entry, index_entry,
										stack_order, seen_reg) == ERROR)
			{ exit_st = ERROR; }
	}

	PUSH_BACK_VECTOR(stack_order, (void*)crt_entry->index);
	del_bitset(depend_node);

	return (exit_st);
}

vector_t*
topological_sort(token_spec_t* spec) {
	vector_t* stack_order = new_vector();
	bitset_t* seen_reg = new_bitset();

	int exit_st = DONE;
	for (size_t i = 0; i < SIZE_VECTOR(spec->entry_lst); ++i) {
		if (IS_PRESENT(seen_reg, i))
			{ continue; }
		token_entry_t* crt_entry = (token_entry_t*)
										AT_VECTOR(spec->entry_lst, i);
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

