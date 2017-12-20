#ifndef _FGFL_GRM_H
#define _FGFL_GRM_H

#define ONLY_TOKEN	
#include "fgfx.lex.h"
#undef	ONLY_TOKEN

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

//Various macro representing production.
#define P_NFGFL_Nsection_NFGFL_PRIME	1
#define P_NFGFL_PRIME_Nsection_NFGFL_PRIME	2
#define P_NFGFL_PRIME	3
#define P_Nsection_Nskip_section	4
#define P_Nsection_Ntoken_section	5
#define P_Nsection_Nkeyword_section	6
#define P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI	7
#define P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI	8
#define P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI	9
#define P_Nregex_list_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list	10
#define P_Nregex_list	11
#define P_Nregex_assign_TEQUAL_TREGEX	12
#define P_Nregex_assign_TARROW_TIGCASE	13
#define P_Nkeyword_list_Natom_FGFL_Nkeyword_list_PRIME	14
#define P_Nkeyword_list	15
#define P_Nkeyword_list_PRIME_TCOMMA_Nkeyword_list	16
#define P_Nkeyword_list_PRIME	17
#define P_Natom_FGFL_TGLOBAL_TOK	18
#define P_Natom_FGFL_TLOCAL_TOK	19


#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

static int
fgfl_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFL][TKEYWORD]=P_NFGFL_Nsection_NFGFL_PRIME, [N_FGFL][TTOKEN]=P_NFGFL_Nsection_NFGFL_PRIME, [N_FGFL][TSKIP]=P_NFGFL_Nsection_NFGFL_PRIME, 
	[N_section][TKEYWORD]=P_Nsection_Nkeyword_section, [N_section][TTOKEN]=P_Nsection_Ntoken_section, [N_section][TSKIP]=P_Nsection_Nskip_section, 
	[N_FGFL_PRIME][TEOF]=P_NFGFL_PRIME, [N_FGFL_PRIME][TKEYWORD]=P_NFGFL_PRIME_Nsection_NFGFL_PRIME, [N_FGFL_PRIME][TTOKEN]=P_NFGFL_PRIME_Nsection_NFGFL_PRIME, [N_FGFL_PRIME][TSKIP]=P_NFGFL_PRIME_Nsection_NFGFL_PRIME, 
	[N_skip_section][TSKIP]=P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI, 
	[N_token_section][TTOKEN]=P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI, 
	[N_keyword_section][TKEYWORD]=P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI, 
	[N_regex_list][TRBRACE]=P_Nregex_list, [N_regex_list][TLOCAL_TOK]=P_Nregex_list_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list, [N_regex_list][TGLOBAL_TOK]=P_Nregex_list_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list, 
	[N_keyword_list][TRBRACE]=P_Nkeyword_list, [N_keyword_list][TLOCAL_TOK]=P_Nkeyword_list_Natom_FGFL_Nkeyword_list_PRIME, [N_keyword_list][TGLOBAL_TOK]=P_Nkeyword_list_Natom_FGFL_Nkeyword_list_PRIME, 
	[N_atom_FGFL][TLOCAL_TOK]=P_Natom_FGFL_TLOCAL_TOK, [N_atom_FGFL][TGLOBAL_TOK]=P_Natom_FGFL_TGLOBAL_TOK, 
	[N_regex_assign][TARROW]=P_Nregex_assign_TARROW_TIGCASE, [N_regex_assign][TEQUAL]=P_Nregex_assign_TEQUAL_TREGEX, 
	[N_keyword_list_PRIME][TRBRACE]=P_Nkeyword_list_PRIME, [N_keyword_list_PRIME][TCOMMA]=P_Nkeyword_list_PRIME_TCOMMA_Nkeyword_list, 
};

#if 0
enum{
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
lr_action_table[40][TOTAL_TOKEN] = {
/*   0 */	{[TKEYWORD]=SHIFT(8),[TTOKEN]=SHIFT(7),[TSKIP]=SHIFT(6),},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[TKEYWORD]=SHIFT(8),[TTOKEN]=SHIFT(7),[TSKIP]=SHIFT(6),[TEOF]=REDUCE(P_NFGFL_PRIME),},
/*   3 */	{[TEOF]=REDUCE(P_Nsection_Nskip_section),[TKEYWORD]=REDUCE(P_Nsection_Nskip_section),[TTOKEN]=REDUCE(P_Nsection_Nskip_section),[TSKIP]=REDUCE(P_Nsection_Nskip_section),},
/*   4 */	{[TEOF]=REDUCE(P_Nsection_Ntoken_section),[TKEYWORD]=REDUCE(P_Nsection_Ntoken_section),[TTOKEN]=REDUCE(P_Nsection_Ntoken_section),[TSKIP]=REDUCE(P_Nsection_Ntoken_section),},
/*   5 */	{[TEOF]=REDUCE(P_Nsection_Nkeyword_section),[TKEYWORD]=REDUCE(P_Nsection_Nkeyword_section),[TTOKEN]=REDUCE(P_Nsection_Nkeyword_section),[TSKIP]=REDUCE(P_Nsection_Nkeyword_section),},
/*   6 */	{[TLBRACE]=SHIFT(11),},
/*   7 */	{[TLBRACE]=SHIFT(10),},
/*   8 */	{[TLBRACE]=SHIFT(9),},
/*   9 */	{[TLOCAL_TOK]=SHIFT(23),[TGLOBAL_TOK]=SHIFT(22),[TRBRACE]=REDUCE(P_Nkeyword_list),},
/*  10 */	{[TLOCAL_TOK]=SHIFT(17),[TGLOBAL_TOK]=SHIFT(16),[TRBRACE]=REDUCE(P_Nregex_list),},
/*  11 */	{[TLOCAL_TOK]=SHIFT(17),[TGLOBAL_TOK]=SHIFT(16),[TRBRACE]=REDUCE(P_Nregex_list),},
/*  12 */	{[TEOF]=REDUCE(P_NFGFL_Nsection_NFGFL_PRIME),},
/*  13 */	{[TKEYWORD]=SHIFT(8),[TTOKEN]=SHIFT(7),[TSKIP]=SHIFT(6),[TEOF]=REDUCE(P_NFGFL_PRIME),},
/*  14 */	{[TEOF]=REDUCE(P_NFGFL_PRIME_Nsection_NFGFL_PRIME),},
/*  15 */	{[TARROW]=SHIFT(31),[TEQUAL]=SHIFT(30),},
/*  16 */	{[TARROW]=REDUCE(P_Natom_FGFL_TGLOBAL_TOK),[TEQUAL]=REDUCE(P_Natom_FGFL_TGLOBAL_TOK),},
/*  17 */	{[TARROW]=REDUCE(P_Natom_FGFL_TLOCAL_TOK),[TEQUAL]=REDUCE(P_Natom_FGFL_TLOCAL_TOK),},
/*  18 */	{[TRBRACE]=SHIFT(28),},
/*  19 */	{[TRBRACE]=SHIFT(27),},
/*  20 */	{[TRBRACE]=SHIFT(26),},
/*  21 */	{[TCOMMA]=SHIFT(25),[TRBRACE]=REDUCE(P_Nkeyword_list_PRIME),},
/*  22 */	{[TCOMMA]=REDUCE(P_Natom_FGFL_TGLOBAL_TOK),[TRBRACE]=REDUCE(P_Natom_FGFL_TGLOBAL_TOK),},
/*  23 */	{[TCOMMA]=REDUCE(P_Natom_FGFL_TLOCAL_TOK),[TRBRACE]=REDUCE(P_Natom_FGFL_TLOCAL_TOK),},
/*  24 */	{[TRBRACE]=REDUCE(P_Nkeyword_list_Natom_FGFL_Nkeyword_list_PRIME),},
/*  25 */	{[TLOCAL_TOK]=SHIFT(23),[TGLOBAL_TOK]=SHIFT(22),[TRBRACE]=REDUCE(P_Nkeyword_list),},
/*  26 */	{[TSEMI]=SHIFT(37),},
/*  27 */	{[TSEMI]=SHIFT(36),},
/*  28 */	{[TSEMI]=SHIFT(35),},
/*  29 */	{[TSEMI]=SHIFT(34),},
/*  30 */	{[TREGEX]=SHIFT(33),},
/*  31 */	{[TIGCASE]=SHIFT(32),},
/*  32 */	{[TSEMI]=REDUCE(P_Nregex_assign_TARROW_TIGCASE),},
/*  33 */	{[TSEMI]=REDUCE(P_Nregex_assign_TEQUAL_TREGEX),},
/*  34 */	{[TLOCAL_TOK]=SHIFT(17),[TGLOBAL_TOK]=SHIFT(16),[TRBRACE]=REDUCE(P_Nregex_list),},
/*  35 */	{[TEOF]=REDUCE(P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TKEYWORD]=REDUCE(P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TTOKEN]=REDUCE(P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TSKIP]=REDUCE(P_Nskip_section_TSKIP_TLBRACE_Nregex_list_TRBRACE_TSEMI),},
/*  36 */	{[TEOF]=REDUCE(P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TKEYWORD]=REDUCE(P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TTOKEN]=REDUCE(P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI),[TSKIP]=REDUCE(P_Ntoken_section_TTOKEN_TLBRACE_Nregex_list_TRBRACE_TSEMI),},
/*  37 */	{[TEOF]=REDUCE(P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI),[TKEYWORD]=REDUCE(P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI),[TTOKEN]=REDUCE(P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI),[TSKIP]=REDUCE(P_Nkeyword_section_TKEYWORD_TLBRACE_Nkeyword_list_TRBRACE_TSEMI),},
/*  38 */	{[TRBRACE]=REDUCE(P_Nkeyword_list_PRIME_TCOMMA_Nkeyword_list),},
/*  39 */	{[TRBRACE]=REDUCE(P_Nregex_list_Natom_FGFL_Nregex_assign_TSEMI_Nregex_list),},
};

static int
lr_goto_table[40][TOTAL_TOKEN] = {
/*   0 */	{[N_keyword_section]=GOTO(5),[N_token_section]=GOTO(4),[N_skip_section]=GOTO(3),[N_section]=GOTO(2),[N_FGFL]=GOTO(1),},
/*   1 */	{},
/*   2 */	{[N_section]=GOTO(13),[N_FGFL_PRIME]=GOTO(12),[N_keyword_section]=GOTO(5),[N_token_section]=GOTO(4),[N_skip_section]=GOTO(3),},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[N_atom_FGFL]=GOTO(21),[N_keyword_list]=GOTO(20),},
/*  10 */	{[N_atom_FGFL]=GOTO(15),[N_regex_list]=GOTO(19),},
/*  11 */	{[N_regex_list]=GOTO(18),[N_atom_FGFL]=GOTO(15),},
/*  12 */	{},
/*  13 */	{[N_FGFL_PRIME]=GOTO(14),[N_section]=GOTO(13),[N_keyword_section]=GOTO(5),[N_token_section]=GOTO(4),[N_skip_section]=GOTO(3),},
/*  14 */	{},
/*  15 */	{[N_regex_assign]=GOTO(29),},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{},
/*  21 */	{[N_keyword_list_PRIME]=GOTO(24),},
/*  22 */	{},
/*  23 */	{},
/*  24 */	{},
/*  25 */	{[N_keyword_list]=GOTO(38),[N_atom_FGFL]=GOTO(21),},
/*  26 */	{},
/*  27 */	{},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{},
/*  31 */	{},
/*  32 */	{},
/*  33 */	{},
/*  34 */	{[N_regex_list]=GOTO(39),[N_atom_FGFL]=GOTO(15),},
/*  35 */	{},
/*  36 */	{},
/*  37 */	{},
/*  38 */	{},
/*  39 */	{},
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

#endif /* _FGFL_GRM_H */
