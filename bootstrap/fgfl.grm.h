#ifndef _FGFL_GRM_H_
#define _FGFL_GRM_H_

#define _ONLY_TOKEN_	
#include "fgfx.lex.h"
#undef	_ONLY_TOKEN_

enum {
	N_FGFL,
	N_section,
	N_FGFL_PRIME,
	N_skip_section,
	N_token_section,
	N_keyword_section,
	N_regex_list,
	N_keyword_list,
	N_atom_FGFL,
	N_regex_assign,
	N_keyword_list_PRIME,
};

#define TOTAL_NTER	11

enum {
	P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME,
	P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME,
	P_LHS_NFGFL_PRIME_RHS,
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

#define TOTAL_PROD	20

#if 0

static int
fgfl_rhs_prod_table[TOTAL_PROD] = {
	[P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME] = 2,
	[P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME] = 2,
	[P_LHS_NFGFL_PRIME_RHS] = 0,
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
	[P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME] = N_FGFL,
	[P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME] = N_FGFL_PRIME,
	[P_LHS_NFGFL_PRIME_RHS] = N_FGFL_PRIME,
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

#endif

#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

static int
fgfl_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFL][T_KEYWORD]=P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME, [N_FGFL][T_TOKEN]=P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME, [N_FGFL][T_SKIP]=P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME, 
	[N_section][T_KEYWORD]=P_LHS_Nsection_RHS_Nkeyword_section, [N_section][T_TOKEN]=P_LHS_Nsection_RHS_Ntoken_section, [N_section][T_SKIP]=P_LHS_Nsection_RHS_Nskip_section, 
	[N_FGFL_PRIME][T_EOF]=P_LHS_NFGFL_PRIME_RHS, [N_FGFL_PRIME][T_KEYWORD]=P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME, [N_FGFL_PRIME][T_TOKEN]=P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME, [N_FGFL_PRIME][T_SKIP]=P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME, 
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
fgfl_lr_action_table[38][TOTAL_TOKEN] = {
/*   0 */	{[T_KEYWORD]=SHIFT(8), [T_TOKEN]=SHIFT(7), [T_SKIP]=SHIFT(6)},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[T_KEYWORD]=SHIFT(8), [T_TOKEN]=SHIFT(7), [T_SKIP]=SHIFT(6), [T_EOF]=REDUCE(P_LHS_NFGFL_PRIME_RHS)},
/*   3 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Nskip_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Nskip_section)},
/*   4 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Ntoken_section)},
/*   5 */	{[T_EOF]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_KEYWORD]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_TOKEN]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section), [T_SKIP]=REDUCE(P_LHS_Nsection_RHS_Nkeyword_section)},
/*   6 */	{[T_LBRACE]=SHIFT(11)},
/*   7 */	{[T_LBRACE]=SHIFT(10)},
/*   8 */	{[T_LBRACE]=SHIFT(9)},
/*   9 */	{[T_LOCAL_TOK]=SHIFT(17), [T_GLOBAL_TOK]=SHIFT(16), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS)},
/*  10 */	{[T_LOCAL_TOK]=SHIFT(17), [T_GLOBAL_TOK]=SHIFT(16), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  11 */	{[T_LOCAL_TOK]=SHIFT(17), [T_GLOBAL_TOK]=SHIFT(16), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  12 */	{[T_EOF]=REDUCE(P_LHS_NFGFL_RHS_Nsection_NFGFL_PRIME)},
/*  13 */	{[T_KEYWORD]=SHIFT(8), [T_TOKEN]=SHIFT(7), [T_SKIP]=SHIFT(6), [T_EOF]=REDUCE(P_LHS_NFGFL_PRIME_RHS)},
/*  14 */	{[T_EOF]=REDUCE(P_LHS_NFGFL_PRIME_RHS_Nsection_NFGFL_PRIME)},
/*  15 */	{[T_ARROW]=SHIFT(29), [T_EQUAL]=SHIFT(28)},
/*  16 */	{[T_ARROW]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK), [T_EQUAL]=REDUCE(P_LHS_Natom_FGFL_RHS_TGLOBAL_TOK)},
/*  17 */	{[T_ARROW]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK), [T_EQUAL]=REDUCE(P_LHS_Natom_FGFL_RHS_TLOCAL_TOK)},
/*  18 */	{[T_RBRACE]=SHIFT(26)},
/*  19 */	{[T_RBRACE]=SHIFT(25)},
/*  20 */	{[T_RBRACE]=SHIFT(24)},
/*  21 */	{[T_COMMA]=SHIFT(23), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_PRIME_RHS)},
/*  22 */	{[T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS_Natom_FGFL_Nkeyword_list_PRIME)},
/*  23 */	{[T_LOCAL_TOK]=SHIFT(17), [T_GLOBAL_TOK]=SHIFT(16), [T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_RHS)},
/*  24 */	{[T_SEMI]=SHIFT(35)},
/*  25 */	{[T_SEMI]=SHIFT(34)},
/*  26 */	{[T_SEMI]=SHIFT(33)},
/*  27 */	{[T_SEMI]=SHIFT(32)},
/*  28 */	{[T_REGEX]=SHIFT(31)},
/*  29 */	{[T_IGCASE]=SHIFT(30)},
/*  30 */	{[T_SEMI]=REDUCE(P_LHS_Nregex_assign_RHS_TARROW_TIGCASE)},
/*  31 */	{[T_SEMI]=REDUCE(P_LHS_Nregex_assign_RHS_TEQUAL_TREGEX)},
/*  32 */	{[T_LOCAL_TOK]=SHIFT(17), [T_GLOBAL_TOK]=SHIFT(16), [T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS)},
/*  33 */	{[T_EOF]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Nskip_section_RHS_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI)},
/*  34 */	{[T_EOF]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Ntoken_section_RHS_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI)},
/*  35 */	{[T_EOF]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_KEYWORD]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_TOKEN]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI), [T_SKIP]=REDUCE(P_LHS_Nkeyword_section_RHS_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI)},
/*  36 */	{[T_RBRACE]=REDUCE(P_LHS_Nkeyword_list_PRIME_RHS_TCOMMA_Nkeyword_list)},
/*  37 */	{[T_RBRACE]=REDUCE(P_LHS_Nregex_list_RHS_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list)},
};

static int
fgfl_lr_goto_table[38][TOTAL_NTER] = {
/*   0 */	{[N_keyword_section]=GOTO(5), [N_token_section]=GOTO(4), [N_skip_section]=GOTO(3), [N_section]=GOTO(2), [N_FGFL]=GOTO(1)},
/*   1 */	{},
/*   2 */	{[N_section]=GOTO(13), [N_FGFL_PRIME]=GOTO(12), [N_keyword_section]=GOTO(5), [N_token_section]=GOTO(4), [N_skip_section]=GOTO(3)},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[N_atom_FGFL]=GOTO(21), [N_keyword_list]=GOTO(20)},
/*  10 */	{[N_atom_FGFL]=GOTO(15), [N_regex_list]=GOTO(19)},
/*  11 */	{[N_regex_list]=GOTO(18), [N_atom_FGFL]=GOTO(15)},
/*  12 */	{},
/*  13 */	{[N_FGFL_PRIME]=GOTO(14), [N_section]=GOTO(13), [N_keyword_section]=GOTO(5), [N_token_section]=GOTO(4), [N_skip_section]=GOTO(3)},
/*  14 */	{},
/*  15 */	{[N_regex_assign]=GOTO(27)},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{},
/*  21 */	{[N_keyword_list_PRIME]=GOTO(22)},
/*  22 */	{},
/*  23 */	{[N_keyword_list]=GOTO(36), [N_atom_FGFL]=GOTO(21)},
/*  24 */	{},
/*  25 */	{},
/*  26 */	{},
/*  27 */	{},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{},
/*  31 */	{},
/*  32 */	{[N_regex_list]=GOTO(37), [N_atom_FGFL]=GOTO(15)},
/*  33 */	{},
/*  34 */	{},
/*  35 */	{},
/*  36 */	{},
/*  37 */	{},
};

#endif

static int
fgfl_sync_token[TOTAL_NTER][5] = {
	[N_FGFL] = { T_EOF, -1 },
	[N_section] = { T_SKIP, T_TOKEN, T_KEYWORD, T_EOF, -1 },
	[N_FGFL_PRIME] = { T_EOF, -1 },
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
