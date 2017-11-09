#include "dfa.h"

set_t* edges(state_t* st, int symbol) {
	if (!st)
		{ return (NULL); }
	set_t* out_states = EMPTY_SET;
	set_t* it = st->edges;
	while (it) {
		edge_t* edg = (edge_t*)it->item;
		if (edg->symbol == symbol)
			{ add_set(&out_states, edg->out_state); }
		it = it->next;
	}
	return (out_states);
}

set_t* closure(set_t** set_state) {
	if (!set_state)
		{ return (EMPTY_SET); }
	set_t* last = EMPTY_SET;
	do {
		del_set(last);
		last = cpy_set(*set_state);
		set_t* it = last;
		while (it) {
			set_t* pass_state = edges((state_t*)it->item, EPSILON);
			union_set(set_state, pass_state);
			it = it->next;
		}
	} while (!equal_set(*set_state, last));
	del_set(last);
	return (*set_state);
}

set_t* DFAedge(set_t* states, int symbol) {
	set_t* target = EMPTY_SET;
	set_t* it = states;
	while (it) {
		set_t* pass_state = edges((state_t*)it->item, symbol);
		union_set(&target, pass_state);
		del_set(pass_state);
		it = it->next;
	}
	return (closure(&target));
}

static void gen_require_header(void) {
	puts("#include <stdint.h>\n");
}

static void gen_get_tok_id(void) {
	puts(STATIC" "INT" "FUNC(get_tok_from_state, int state)" "BEG_BLOCK);
	puts(TAB FOR(int i = 0, i < SIZE_FINAL_TAB, ++i)" "BEG_BLOCK);
	puts(TAB TAB IF(final_table[i][0] == state));
	puts(TAB TAB TAB BEG_BLOCK" "RETURN(final_table[i][1])""SEMI" "END_BLOCK);
	puts(TAB END_BLOCK);
	puts(TAB RETURN(TNONE)""SEMI);
	puts(END_BLOCK);
}

static void gen_enum(vector_t* elst) {
	puts(ENUM" "BEG_BLOCK);
	puts(TAB"TNONE,");
	for (size_t i = 0; i < size_vector(elst); ++i) {
		token_entry_t* te = (token_entry_t*)at_vector(elst, i);
		if (!te->local)
			{ printf(TAB"T%s,\n", te->name); }
	}
	puts(END_BLOCK""SEMI"\n");
}

static void gen_final_table(vector_t* dfa_states, vector_t* elst) {
	puts("static uint8_t final_table[][2] = {");
	size_t count_final = 0;
	for (size_t i = 1; i < size_vector(dfa_states); ++i) {
		set_t* set_state = (set_t*)at_vector(dfa_states, i);
		int min_tok = 0;
		set_t* it = set_state;
		while (it) {
			state_t* crt_state = (state_t*)(it->item);
			if (crt_state->final) {
				if (!min_tok || min_tok > crt_state->final)
					{ min_tok = crt_state->final; }
				++count_final;
			}
			it = it->next;
		}
		if (min_tok) {
			char* name = ((token_entry_t*)at_vector(elst, min_tok - 1))->name;
			printf(TAB"{%zu, "TAB"T%s},\n", i, name);
		}
	}
	printf("};\n\n#define SIZE_FINAL_TAB %zu\n\n", count_final);
}

static vector_t* gen_state_table(state_t* master) {
	vector_t* dfa_states = vector();
	set_t* start = set(master);

	push_back_vector(dfa_states, EMPTY_SET);
	push_back_vector(dfa_states, closure(&start));

	printf("static uint16_t state_table[][%d] = {\n", CHAR_MAX);
	int j = 0, p = 1;
	while (j <= p) {
		printf(TAB BEG_BLOCK);
		for (int i = MIN_ASCII; i < MAX_ASCII; ++i) {
			set_t* next = DFAedge(at_vector(dfa_states, j), i);
			if (next == EMPTY_SET)
				{ continue; }
			int l = 1;
			for (; l <= p; ++l) {
				if (equal_set(next, at_vector(dfa_states, l)))
					{ break; }
			}
			if (l <= p) {
				printf("[%d]=%d, ", i, l);
				del_set(next);
			}
			else {
				++p;
				printf("[%d]=%d, ", i, p);
				push_back_vector(dfa_states, next);
			}
		}
		++j;
		puts(END_BLOCK",");
	}
	puts(END_BLOCK""SEMI"\n");
	return (dfa_states);
}

void DFAgen(token_spec_t* spec) {
	if (!spec)
		{ return; }
	gen_require_header();
	gen_enum(spec->entry_lst);
	vector_t* dfa_states = gen_state_table(spec->master);
	gen_final_table(dfa_states, spec->entry_lst);
	gen_get_tok_id();
	del_vector(dfa_states);
}

