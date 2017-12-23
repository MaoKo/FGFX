#include <stdio.h>

#include "display_lr.h"
#include "display_dfa.h"
#include "display.h"
#include "lr.h"

void
display_action_enum(int filde) {
	dprintf(filde, ENUM BEG_BLOCK NL);
	dprintf(filde, TAB "_SHIFT" TAB "= 0x1000" COMMA NL);
	dprintf(filde, TAB "_REDUCE" TAB "= 0x2000" COMMA NL);
	dprintf(filde, TAB "_GOTO" TAB "= 0x4000" COMMA NL);
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
display_lr_useful_macro(int filde) {
	dprintf(filde, DEFINE(SHIFT(x),(x | _SHIFT)) NL);
	dprintf(filde, DEFINE(REDUCE(x),(x | _REDUCE)) NL);
	dprintf(filde, DEFINE(GOTO(x),(x | _GOTO)) NL);
	dprintf(filde, DEFINE(ACCEPT, (~0)) NL NL);
	dprintf(filde, DEFINE(LR_START_STATE, 0) NL NL);
}

void
display_action_table(int filde, cfg_t const* cfg, vector_t const* lr1_states) {
	dprintf(filde, STATIC SP INT NL);
	dprintf(filde, "lr_action_table[%zu][" MACRO_TOKEN "] = " BEG_BLOCK NL,
			SIZE_VECTOR(lr1_states));

	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		lr1_state_t* state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
		dprintf(filde, "/* %3zu */" TAB BEG_BLOCK, i);
		trans_list_t* action = state->shift_lst;
		while (action) {
			symbol_t* ter = (symbol_t*)AT_VECTOR(cfg->terminal,
								action->input ^ _SHIFT);
			dprintf(filde, "[" TOKEN_PREFIX SEP "%s]="
					SHIFT_STR "(%d)" COMMA, ter->name, action->state);
			action = action->next;
		}
			
		action = state->reduces;
		while (action) {
			symbol_t* ter = (symbol_t*)AT_VECTOR(cfg->terminal, action->input);
			dprintf(filde, "[" TOKEN_PREFIX "%s]=" REDUCE_STR "(", ter->name);
			display_nproduction_macro(filde, cfg, action->state ^ _REDUCE);		
			dprintf(filde, ")" COMMA);
			action = action->next;
		}

		if (state->accept)
			{ dprintf(filde, "[T_EOF]=" ACCEPT_STR COMMA); }
		dprintf(filde, END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

void
display_goto_table(int filde, cfg_t const* cfg, vector_t const* lr1_states) {
	dprintf(filde, STATIC SP INT NL);
	dprintf(filde, "lr_goto_table[%zu][" MACRO_TOKEN "] = " BEG_BLOCK NL,
			SIZE_VECTOR(lr1_states));

	for (size_t i = 0; i < SIZE_VECTOR(lr1_states); ++i) {
		lr1_state_t* state = (lr1_state_t*)AT_VECTOR(lr1_states, i);
		dprintf(filde, "/* %3zu */" TAB BEG_BLOCK, i);
		trans_list_t* action = state->goto_lst;
		while (action) {
			dprintf(filde, "[");
			display_nter_symbol(filde, cfg, action->input ^ _GOTO, true);
			dprintf(filde, "]=" GOTO_STR "(%d)" COMMA, action->state);

			action = action->next;
		}
		dprintf(filde, END_BLOCK COMMA NL);
	}
	dprintf(filde, END_BLOCK SEMI NL NL);
}

