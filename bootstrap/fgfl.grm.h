#ifndef _FGFL_GRM_H_
#define _FGFL_GRM_H_

#define _ONLY_TOKEN_	
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

enum {
	N_FGFL,
	N_section,
	N_skip_section,
	N_token_section,
	N_keyword_section,
	N_regex_list,
	N_keyword_list,
	N_atom_FGFL,
	N_regex_assign,
	N_keyword_list_PRIME,
};

#define TOTAL_NTER	10

enum {
	P_LHS_NFGFL_RHS_Nsection_NFGFL,
	P_LHS_NFGFL_RHS,
	P_LHS_Nsection_RHS_Nskip_section,
	P_LHS_Nsection_RHS_Ntoken_section,
	P_LHS_Nsection_RHS_Nkeyword_section,
	P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI,
	P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI,
	P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI,
	P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list,
	P_LHS_Nregex_list_RHS,
	P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX,
	P_LHS_Nregex_assign_RHS_TARROW_TIGCASE,
	P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME,
	P_LHS_Nkeyword_list_RHS,
	P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list,
	P_LHS_Nkeyword_list_PRIME_RHS,
	P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK,
	P_LHS_Natom_FGFL_RHS_TLOCAL_TOK,
};

#define TOTAL_PROD	19

static int
fgfl_rhs_prod_table[TOTAL_PROD] = {
	[P_LHS_NFGFL_RHS_Nsection_NFGFL] = 2,
	[P_LHS_NFGFL_RHS] = 0,
	[P_LHS_Nsection_RHS_Nskip_section] = 1,
	[P_LHS_Nsection_RHS_Ntoken_section] = 1,
	[P_LHS_Nsection_RHS_Nkeyword_section] = 1,
	[P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI] = 5,
	[P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI] = 5,
	[P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI] = 5,
	[P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list] = 4,
	[P_LHS_Nregex_list_RHS] = 0,
	[P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX] = 2,
	[P_LHS_Nregex_assign_RHS_TARROW_TIGCASE] = 2,
	[P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME] = 2,
	[P_LHS_Nkeyword_list_RHS] = 0,
	[P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list] = 2,
	[P_LHS_Nkeyword_list_PRIME_RHS] = 0,
	[P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK] = 1,
	[P_LHS_Natom_FGFL_RHS_TLOCAL_TOK] = 1,
};

static int
fgfl_lhs_prod_table[TOTAL_PROD] = {
	[P_LHS_NFGFL_RHS_Nsection_NFGFL] = N_FGFL,
	[P_LHS_NFGFL_RHS] = N_FGFL,
	[P_LHS_Nsection_RHS_Nskip_section] = N_section,
	[P_LHS_Nsection_RHS_Ntoken_section] = N_section,
	[P_LHS_Nsection_RHS_Nkeyword_section] = N_section,
	[P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI] = N_skip_section,
	[P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI] = N_token_section,
	[P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI] = N_keyword_section,
	[P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list] = N_regex_list,
	[P_LHS_Nregex_list_RHS] = N_regex_list,
	[P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX] = N_regex_assign,
	[P_LHS_Nregex_assign_RHS_TARROW_TIGCASE] = N_regex_assign,
	[P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME] = N_keyword_list,
	[P_LHS_Nkeyword_list_RHS] = N_keyword_list,
	[P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list] = N_keyword_list_PRIME,
	[P_LHS_Nkeyword_list_PRIME_RHS] = N_keyword_list_PRIME,
	[P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK] = N_atom_FGFL,
	[P_LHS_Natom_FGFL_RHS_TLOCAL_TOK] = N_atom_FGFL,
};

#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

static int
fgfl_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFL][T_EOF]=P_LHS_NFGFL_RHS, [N_FGFL][T_KEYWORD]=P_LHS_NFGFL_RHS_Nsection_NFGFL, [N_FGFL][T_TOKEN]=P_LHS_NFGFL_RHS_Nsection_NFGFL, [N_FGFL][T_SKIP]=P_LHS_NFGFL_RHS_Nsection_NFGFL, 
	[N_section][T_KEYWORD]=P_LHS_Nsection_RHS_Nkeyword_section, [N_section][T_TOKEN]=P_LHS_Nsection_RHS_Ntoken_section, [N_section][T_SKIP]=P_LHS_Nsection_RHS_Nskip_section, 
	[N_skip_section][T_SKIP]=P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI, 
	[N_token_section][T_TOKEN]=P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI, 
	[N_keyword_section][T_KEYWORD]=P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI, 
	[N_regex_list][T_RBRACE]=P_LHS_Nregex_list_RHS, [N_regex_list][T_LOCAL_TOK]=P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list, [N_regex_list][T_GLOBAL_TOK]=P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list, 
	[N_keyword_list][T_RBRACE]=P_LHS_Nkeyword_list_RHS, [N_keyword_list][T_LOCAL_TOK]=P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME, [N_keyword_list][T_GLOBAL_TOK]=P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME, 
	[N_atom_FGFL][T_LOCAL_TOK]=P_LHS_Natom_FGFL_RHS_TLOCAL_TOK, [N_atom_FGFL][T_GLOBAL_TOK]=P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK, 
	[N_regex_assign][T_ARROW]=P_LHS_Nregex_assign_RHS_TARROW_TIGCASE, [N_regex_assign][T_EQUAL]=P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX, 
	[N_keyword_list_PRIME][T_RBRACE]=P_LHS_Nkeyword_list_PRIME_RHS, [N_keyword_list_PRIME][T_COMMA]=P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list, 
};

#if 0

enum {
	_SHIFT	= 0x1000,
	_REDUCE	= 0x2000,
	_GOTO	= 0x4000,
};

#define SHIFT(x)	(x | _SHIFT)
#define REDUCE(x)	(x | _REDUCE)
#define GOTO(x)	(x | _GOTO)
#define ACCEPT	(~0)

#define LR_START_STATE	0

static int
fgfl_lr_action_table[36][TOTAL_TOKEN] = {
/*   0 */	{[T_KEYWORD]=SHIFT(8), [T_TOKEN]=SHIFT(7), [T_SKIP]=SHIFT(6), [T_EOF]=REDUCE(P_LHS_NFGFL_RHS)},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[T_KEYWORD]=SHIFT(8), [T_TOKEN]=SHIFT(7), [T_SKIP]=SHIFT(6), [T_EOF]=REDUCE(P_LHS_NFGFL_RHS)},
/*   3 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Nskip_section)},
/*   4 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section)},
/*   5 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section)},
/*   6 */	{[T_LBRACE]=SHIFT(11)},
/*   7 */	{[T_LBRACE]=SHIFT(10)},
/*   8 */	{[T_LBRACE]=SHIFT(9)},
/*   9 */	{[T_LOCAL_TOK]=SHIFT(15), [T_GLOBAL_TOK]=SHIFT(14), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS)},
/*  10 */	{[T_LOCAL_TOK]=SHIFT(15), [T_GLOBAL_TOK]=SHIFT(14), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  11 */	{[T_LOCAL_TOK]=SHIFT(15), [T_GLOBAL_TOK]=SHIFT(14), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  12 */	{[T_EOF]=REDUCE(P_LHS_NFGFL_RHS_Nsection_NFGFL)},
/*  13 */	{[T_ARROW]=SHIFT(27), [T_EQUAL]=SHIFT(26)},
/*  14 */	{[T_ARROW]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK), [T_EQUAL]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK), [T_COMMA]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK), [T_RBRACE]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK)},
/*  15 */	{[T_ARROW]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK), [T_EQUAL]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK), [T_COMMA]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK), [T_RBRACE]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK)},
/*  16 */	{[T_RBRACE]=SHIFT(24)},
/*  17 */	{[T_RBRACE]=SHIFT(23)},
/*  18 */	{[T_RBRACE]=SHIFT(22)},
/*  19 */	{[T_COMMA]=SHIFT(21), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_PRIME_RHS)},
/*  20 */	{[T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME)},
/*  21 */	{[T_LOCAL_TOK]=SHIFT(15), [T_GLOBAL_TOK]=SHIFT(14), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS)},
/*  22 */	{[T_SEMI]=SHIFT(33)},
/*  23 */	{[T_SEMI]=SHIFT(32)},
/*  24 */	{[T_SEMI]=SHIFT(31)},
/*  25 */	{[T_SEMI]=SHIFT(30)},
/*  26 */	{[T_REGEX]=SHIFT(29)},
/*  27 */	{[T_IGCASE]=SHIFT(28)},
/*  28 */	{[T_SEMI]=REDUCE(P_LHS_Nregex_assign_RHS_TARROW_TIGCASE)},
/*  29 */	{[T_SEMI]=REDUCE(P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX)},
/*  30 */	{[T_LOCAL_TOK]=SHIFT(15), [T_GLOBAL_TOK]=SHIFT(14), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  31 */	{[T_EOF]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI)},
/*  32 */	{[T_EOF]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI)},
/*  33 */	{[T_EOF]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI)},
/*  34 */	{[T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list)},
/*  35 */	{[T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list)},
};

static int
fgfl_lr_goto_table[36][TOTAL_NTER] = {
/*   0 */	{[N_keyword_section]=GOTO(5), [N_token_section]=GOTO(4), [N_skip_section]=GOTO(3), [N_section]=GOTO(2), [N_FGFL]=GOTO(1)},
/*   1 */	{},
/*   2 */	{[N_FGFL]=GOTO(12), [N_keyword_section]=GOTO(5), [N_token_section]=GOTO(4), [N_skip_section]=GOTO(3), [N_section]=GOTO(2)},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[N_atom_FGFL]=GOTO(19), [N_keyword_list]=GOTO(18)},
/*  10 */	{[N_atom_FGFL]=GOTO(13), [N_regex_list]=GOTO(17)},
/*  11 */	{[N_regex_list]=GOTO(16), [N_atom_FGFL]=GOTO(13)},
/*  12 */	{},
/*  13 */	{[N_regex_assign]=GOTO(25)},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{[N_keyword_list_PRIME]=GOTO(20)},
/*  20 */	{},
/*  21 */	{[N_keyword_list]=GOTO(34), [N_atom_FGFL]=GOTO(19)},
/*  22 */	{},
/*  23 */	{},
/*  24 */	{},
/*  25 */	{},
/*  26 */	{},
/*  27 */	{},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{[N_regex_list]=GOTO(35), [N_atom_FGFL]=GOTO(13)},
/*  31 */	{},
/*  32 */	{},
/*  33 */	{},
/*  34 */	{},
/*  35 */	{},
};

#endif

static int
fgfl_sync_token[TOTAL_NTER][5] = {
	[N_FGFL] = { T_EOF, -1 },
	[N_section] = { T_SKIP, T_TOKEN, T_KEYWORD, T_EOF, -1 },
	[N_skip_section] = { T_SKIP, T_TOKEN, T_KEYWORD, T_EOF, -1 },
	[N_token_section] = { T_SKIP, T_TOKEN, T_KEYWORD, T_EOF, -1 },
	[N_keyword_section] = { T_SKIP, T_TOKEN, T_KEYWORD, T_EOF, -1 },
	[N_regex_list] = { T_RBRACE, -1 },
	[N_keyword_list] = { T_RBRACE, -1 },
	[N_atom_FGFL] = { T_RBRACE, T_COMMA, T_EQUAL, T_ARROW, -1 },
	[N_regex_assign] = { T_SEMI, -1 },
	[N_keyword_list_PRIME] = { T_RBRACE, -1 },
};

#endif /* _FGFL_GRM_H_ */
