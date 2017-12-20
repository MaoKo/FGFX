#ifndef _FGFP_GRM_H
#define _FGFP_GRM_H

#define ONLY_TOKEN	
#include "fgfx.lex.h"
#undef	ONLY_TOKEN

enum {
	N_FGFP,
	N_section,
	N_FGFP_PRIME,
	N_extern_section,
	N_production_section,
	N_alias_section,
	N_path_list,
	N_path_list_PRIME,
	N_alias_list,
	N_alias_list_PRIME,
	N_production_list,
	N_production,
	N_production_list_PRIME,
	N_follow_prod,
	N_lhs,
	N_opt_list,
	N_lhs_PRIME,
	N_symbol_list,
	N_symbol,
	N_symbol_list_PRIME,
};

#define TOTAL_NTER	20

//Various macro representing production.
#define P_NFGFP_Nsection_NFGFP_PRIME	1
#define P_NFGFP_PRIME_Nsection_NFGFP_PRIME	2
#define P_NFGFP_PRIME	3
#define P_Nsection_Nextern_section	4
#define P_Nsection_Nproduction_section	5
#define P_Nsection_Nalias_section	6
#define P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI	7
#define P_Npath_list_TLITERAL_Npath_list_PRIME	8
#define P_Npath_list	9
#define P_Npath_list_PRIME_TCOMMA_Npath_list	10
#define P_Npath_list_PRIME	11
#define P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI	12
#define P_Nalias_list_TLPAREN_TGLOBAL_TOK_TBARROW_TLITERAL_TRPAREN_Nalias_list_PRIME	13
#define P_Nalias_list	14
#define P_Nalias_list_PRIME_TCOMMA_Nalias_list	15
#define P_Nalias_list_PRIME	16
#define P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI	17
#define P_Nproduction_list_Nproduction_Nproduction_list_PRIME	18
#define P_Nproduction_list	19
#define P_Nproduction_list_PRIME_Nproduction_Nproduction_list_PRIME	20
#define P_Nproduction_list_PRIME	21
#define P_Nproduction_TNTER_Nfollow_prod_TSEMI	22
#define P_Nfollow_prod_TEQUAL_TSTART	23
#define P_Nfollow_prod_TARROW_Nlhs	24
#define P_Nlhs_Nopt_list_Nlhs_PRIME	25
#define P_Nlhs_PRIME_TUNION_Nopt_list_Nlhs_PRIME	26
#define P_Nlhs_PRIME	27
#define P_Nopt_list_Nsymbol_list	28
#define P_Nopt_list_TEMPTY	29
#define P_Nopt_list	30
#define P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME	31
#define P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME	32
#define P_Nsymbol_list_PRIME	33
#define P_Nsymbol_TGLOBAL_TOK	34
#define P_Nsymbol_TNTER	35
#define P_Nsymbol_TLITERAL	36


#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFP

static int
fgfp_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFP][TPRODUCTION]=P_NFGFP_Nsection_NFGFP_PRIME, [N_FGFP][TALIAS]=P_NFGFP_Nsection_NFGFP_PRIME, [N_FGFP][TEXTERN]=P_NFGFP_Nsection_NFGFP_PRIME, 
	[N_section][TALIAS]=P_Nsection_Nalias_section, [N_section][TPRODUCTION]=P_Nsection_Nproduction_section, [N_section][TEXTERN]=P_Nsection_Nextern_section, 
	[N_FGFP_PRIME][TEOF]=P_NFGFP_PRIME, [N_FGFP_PRIME][TPRODUCTION]=P_NFGFP_PRIME_Nsection_NFGFP_PRIME, [N_FGFP_PRIME][TALIAS]=P_NFGFP_PRIME_Nsection_NFGFP_PRIME, [N_FGFP_PRIME][TEXTERN]=P_NFGFP_PRIME_Nsection_NFGFP_PRIME, 
	[N_extern_section][TEXTERN]=P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI, 
	[N_production_section][TPRODUCTION]=P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI, 
	[N_alias_section][TALIAS]=P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI, 
	[N_path_list][TRBRACE]=P_Npath_list, [N_path_list][TLITERAL]=P_Npath_list_TLITERAL_Npath_list_PRIME, 
	[N_path_list_PRIME][TRBRACE]=P_Npath_list_PRIME, [N_path_list_PRIME][TCOMMA]=P_Npath_list_PRIME_TCOMMA_Npath_list, 
	[N_alias_list][TRBRACE]=P_Nalias_list, [N_alias_list][TLPAREN]=P_Nalias_list_TLPAREN_TGLOBAL_TOK_TBARROW_TLITERAL_TRPAREN_Nalias_list_PRIME, 
	[N_alias_list_PRIME][TRBRACE]=P_Nalias_list_PRIME, [N_alias_list_PRIME][TCOMMA]=P_Nalias_list_PRIME_TCOMMA_Nalias_list, 
	[N_production_list][TRBRACE]=P_Nproduction_list, [N_production_list][TNTER]=P_Nproduction_list_Nproduction_Nproduction_list_PRIME, 
	[N_production][TNTER]=P_Nproduction_TNTER_Nfollow_prod_TSEMI, 
	[N_production_list_PRIME][TRBRACE]=P_Nproduction_list_PRIME, [N_production_list_PRIME][TNTER]=P_Nproduction_list_PRIME_Nproduction_Nproduction_list_PRIME, 
	[N_follow_prod][TARROW]=P_Nfollow_prod_TARROW_Nlhs, [N_follow_prod][TEQUAL]=P_Nfollow_prod_TEQUAL_TSTART, 
	[N_lhs][TSEMI]=P_Nlhs_Nopt_list_Nlhs_PRIME, [N_lhs][TEMPTY]=P_Nlhs_Nopt_list_Nlhs_PRIME, [N_lhs][TUNION]=P_Nlhs_Nopt_list_Nlhs_PRIME, [N_lhs][TNTER]=P_Nlhs_Nopt_list_Nlhs_PRIME, [N_lhs][TGLOBAL_TOK]=P_Nlhs_Nopt_list_Nlhs_PRIME, [N_lhs][TLITERAL]=P_Nlhs_Nopt_list_Nlhs_PRIME, 
	[N_opt_list][TUNION]=P_Nopt_list, [N_opt_list][TSEMI]=P_Nopt_list, [N_opt_list][TEMPTY]=P_Nopt_list_TEMPTY, [N_opt_list][TNTER]=P_Nopt_list_Nsymbol_list, [N_opt_list][TGLOBAL_TOK]=P_Nopt_list_Nsymbol_list, [N_opt_list][TLITERAL]=P_Nopt_list_Nsymbol_list, 
	[N_lhs_PRIME][TSEMI]=P_Nlhs_PRIME, [N_lhs_PRIME][TUNION]=P_Nlhs_PRIME_TUNION_Nopt_list_Nlhs_PRIME, 
	[N_symbol_list][TNTER]=P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME, [N_symbol_list][TGLOBAL_TOK]=P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME, [N_symbol_list][TLITERAL]=P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME, 
	[N_symbol][TLITERAL]=P_Nsymbol_TLITERAL, [N_symbol][TNTER]=P_Nsymbol_TNTER, [N_symbol][TGLOBAL_TOK]=P_Nsymbol_TGLOBAL_TOK, 
	[N_symbol_list_PRIME][TUNION]=P_Nsymbol_list_PRIME, [N_symbol_list_PRIME][TSEMI]=P_Nsymbol_list_PRIME, [N_symbol_list_PRIME][TNTER]=P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME, [N_symbol_list_PRIME][TGLOBAL_TOK]=P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME, [N_symbol_list_PRIME][TLITERAL]=P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME, 
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
lr_action_table[61][TOTAL_TOKEN] = {
/*   0 */	{[TALIAS]=SHIFT(8),[TPRODUCTION]=SHIFT(7),[TEXTERN]=SHIFT(6),},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[TALIAS]=SHIFT(8),[TPRODUCTION]=SHIFT(7),[TEXTERN]=SHIFT(6),[TEOF]=REDUCE(P_NFGFP_PRIME),},
/*   3 */	{[TEOF]=REDUCE(P_Nsection_Nextern_section),[TPRODUCTION]=REDUCE(P_Nsection_Nextern_section),[TALIAS]=REDUCE(P_Nsection_Nextern_section),[TEXTERN]=REDUCE(P_Nsection_Nextern_section),},
/*   4 */	{[TEOF]=REDUCE(P_Nsection_Nproduction_section),[TPRODUCTION]=REDUCE(P_Nsection_Nproduction_section),[TALIAS]=REDUCE(P_Nsection_Nproduction_section),[TEXTERN]=REDUCE(P_Nsection_Nproduction_section),},
/*   5 */	{[TEOF]=REDUCE(P_Nsection_Nalias_section),[TPRODUCTION]=REDUCE(P_Nsection_Nalias_section),[TALIAS]=REDUCE(P_Nsection_Nalias_section),[TEXTERN]=REDUCE(P_Nsection_Nalias_section),},
/*   6 */	{[TLBRACE]=SHIFT(11),},
/*   7 */	{[TLBRACE]=SHIFT(10),},
/*   8 */	{[TLBRACE]=SHIFT(9),},
/*   9 */	{[TLPAREN]=SHIFT(21),[TRBRACE]=REDUCE(P_Nalias_list),},
/*  10 */	{[TNTER]=SHIFT(19),[TRBRACE]=REDUCE(P_Nproduction_list),},
/*  11 */	{[TLITERAL]=SHIFT(16),[TRBRACE]=REDUCE(P_Npath_list),},
/*  12 */	{[TEOF]=REDUCE(P_NFGFP_Nsection_NFGFP_PRIME),},
/*  13 */	{[TALIAS]=SHIFT(8),[TPRODUCTION]=SHIFT(7),[TEXTERN]=SHIFT(6),[TEOF]=REDUCE(P_NFGFP_PRIME),},
/*  14 */	{[TEOF]=REDUCE(P_NFGFP_PRIME_Nsection_NFGFP_PRIME),},
/*  15 */	{[TRBRACE]=SHIFT(32),},
/*  16 */	{[TCOMMA]=SHIFT(31),[TRBRACE]=REDUCE(P_Npath_list_PRIME),},
/*  17 */	{[TRBRACE]=SHIFT(29),},
/*  18 */	{[TNTER]=SHIFT(19),[TRBRACE]=REDUCE(P_Nproduction_list_PRIME),},
/*  19 */	{[TARROW]=SHIFT(26),[TEQUAL]=SHIFT(25),},
/*  20 */	{[TRBRACE]=SHIFT(23),},
/*  21 */	{[TGLOBAL_TOK]=SHIFT(22),},
/*  22 */	{[TBARROW]=SHIFT(48),},
/*  23 */	{[TSEMI]=SHIFT(47),},
/*  24 */	{[TSEMI]=SHIFT(46),},
/*  25 */	{[TSTART]=SHIFT(45),},
/*  26 */	{[TLITERAL]=SHIFT(44),[TNTER]=SHIFT(43),[TGLOBAL_TOK]=SHIFT(42),[TEMPTY]=SHIFT(40),[TUNION]=REDUCE(P_Nopt_list),[TSEMI]=REDUCE(P_Nopt_list),},
/*  27 */	{[TRBRACE]=REDUCE(P_Nproduction_list_Nproduction_Nproduction_list_PRIME),},
/*  28 */	{[TNTER]=SHIFT(19),[TRBRACE]=REDUCE(P_Nproduction_list_PRIME),},
/*  29 */	{[TSEMI]=SHIFT(35),},
/*  30 */	{[TRBRACE]=REDUCE(P_Npath_list_TLITERAL_Npath_list_PRIME),},
/*  31 */	{[TLITERAL]=SHIFT(16),[TRBRACE]=REDUCE(P_Npath_list),},
/*  32 */	{[TSEMI]=SHIFT(33),},
/*  33 */	{[TEOF]=REDUCE(P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI),[TPRODUCTION]=REDUCE(P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI),[TALIAS]=REDUCE(P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI),[TEXTERN]=REDUCE(P_Nextern_section_TEXTERN_TLBRACE_Npath_list_TRBRACE_TSEMI),},
/*  34 */	{[TRBRACE]=REDUCE(P_Npath_list_PRIME_TCOMMA_Npath_list),},
/*  35 */	{[TEOF]=REDUCE(P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI),[TPRODUCTION]=REDUCE(P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI),[TALIAS]=REDUCE(P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI),[TEXTERN]=REDUCE(P_Nproduction_section_TPRODUCTION_TLBRACE_Nproduction_list_TRBRACE_TSEMI),},
/*  36 */	{[TRBRACE]=REDUCE(P_Nproduction_list_PRIME_Nproduction_Nproduction_list_PRIME),},
/*  37 */	{[TSEMI]=REDUCE(P_Nfollow_prod_TARROW_Nlhs),},
/*  38 */	{[TUNION]=SHIFT(53),[TSEMI]=REDUCE(P_Nlhs_PRIME),},
/*  39 */	{[TUNION]=REDUCE(P_Nopt_list_Nsymbol_list),[TSEMI]=REDUCE(P_Nopt_list_Nsymbol_list),},
/*  40 */	{[TUNION]=REDUCE(P_Nopt_list_TEMPTY),[TSEMI]=REDUCE(P_Nopt_list_TEMPTY),},
/*  41 */	{[TLITERAL]=SHIFT(44),[TNTER]=SHIFT(43),[TGLOBAL_TOK]=SHIFT(42),[TUNION]=REDUCE(P_Nsymbol_list_PRIME),[TSEMI]=REDUCE(P_Nsymbol_list_PRIME),},
/*  42 */	{[TUNION]=REDUCE(P_Nsymbol_TGLOBAL_TOK),[TSEMI]=REDUCE(P_Nsymbol_TGLOBAL_TOK),[TNTER]=REDUCE(P_Nsymbol_TGLOBAL_TOK),[TGLOBAL_TOK]=REDUCE(P_Nsymbol_TGLOBAL_TOK),[TLITERAL]=REDUCE(P_Nsymbol_TGLOBAL_TOK),},
/*  43 */	{[TUNION]=REDUCE(P_Nsymbol_TNTER),[TSEMI]=REDUCE(P_Nsymbol_TNTER),[TNTER]=REDUCE(P_Nsymbol_TNTER),[TGLOBAL_TOK]=REDUCE(P_Nsymbol_TNTER),[TLITERAL]=REDUCE(P_Nsymbol_TNTER),},
/*  44 */	{[TUNION]=REDUCE(P_Nsymbol_TLITERAL),[TSEMI]=REDUCE(P_Nsymbol_TLITERAL),[TNTER]=REDUCE(P_Nsymbol_TLITERAL),[TGLOBAL_TOK]=REDUCE(P_Nsymbol_TLITERAL),[TLITERAL]=REDUCE(P_Nsymbol_TLITERAL),},
/*  45 */	{[TSEMI]=REDUCE(P_Nfollow_prod_TEQUAL_TSTART),},
/*  46 */	{[TRBRACE]=REDUCE(P_Nproduction_TNTER_Nfollow_prod_TSEMI),[TNTER]=REDUCE(P_Nproduction_TNTER_Nfollow_prod_TSEMI),},
/*  47 */	{[TEOF]=REDUCE(P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI),[TPRODUCTION]=REDUCE(P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI),[TALIAS]=REDUCE(P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI),[TEXTERN]=REDUCE(P_Nalias_section_TALIAS_TLBRACE_Nalias_list_TRBRACE_TSEMI),},
/*  48 */	{[TLITERAL]=SHIFT(49),},
/*  49 */	{[TRPAREN]=SHIFT(56),},
/*  50 */	{[TUNION]=REDUCE(P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME),[TSEMI]=REDUCE(P_Nsymbol_list_Nsymbol_Nsymbol_list_PRIME),},
/*  51 */	{[TLITERAL]=SHIFT(44),[TNTER]=SHIFT(43),[TGLOBAL_TOK]=SHIFT(42),[TUNION]=REDUCE(P_Nsymbol_list_PRIME),[TSEMI]=REDUCE(P_Nsymbol_list_PRIME),},
/*  52 */	{[TSEMI]=REDUCE(P_Nlhs_Nopt_list_Nlhs_PRIME),},
/*  53 */	{[TLITERAL]=SHIFT(44),[TNTER]=SHIFT(43),[TGLOBAL_TOK]=SHIFT(42),[TEMPTY]=SHIFT(40),[TUNION]=REDUCE(P_Nopt_list),[TSEMI]=REDUCE(P_Nopt_list),},
/*  54 */	{[TUNION]=SHIFT(53),[TSEMI]=REDUCE(P_Nlhs_PRIME),},
/*  55 */	{[TUNION]=REDUCE(P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME),[TSEMI]=REDUCE(P_Nsymbol_list_PRIME_Nsymbol_Nsymbol_list_PRIME),},
/*  56 */	{[TCOMMA]=SHIFT(58),[TRBRACE]=REDUCE(P_Nalias_list_PRIME),},
/*  57 */	{[TRBRACE]=REDUCE(P_Nalias_list_TLPAREN_TGLOBAL_TOK_TBARROW_TLITERAL_TRPAREN_Nalias_list_PRIME),},
/*  58 */	{[TLPAREN]=SHIFT(21),[TRBRACE]=REDUCE(P_Nalias_list),},
/*  59 */	{[TSEMI]=REDUCE(P_Nlhs_PRIME_TUNION_Nopt_list_Nlhs_PRIME),},
/*  60 */	{[TRBRACE]=REDUCE(P_Nalias_list_PRIME_TCOMMA_Nalias_list),},
};

static int
lr_goto_table[61][TOTAL_TOKEN] = {
/*   0 */	{[N_alias_section]=GOTO(5),[N_production_section]=GOTO(4),[N_extern_section]=GOTO(3),[N_section]=GOTO(2),[N_FGFP]=GOTO(1),},
/*   1 */	{},
/*   2 */	{[N_section]=GOTO(13),[N_FGFP_PRIME]=GOTO(12),[N_alias_section]=GOTO(5),[N_production_section]=GOTO(4),[N_extern_section]=GOTO(3),},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[N_alias_list]=GOTO(20),},
/*  10 */	{[N_production]=GOTO(18),[N_production_list]=GOTO(17),},
/*  11 */	{[N_path_list]=GOTO(15),},
/*  12 */	{},
/*  13 */	{[N_FGFP_PRIME]=GOTO(14),[N_section]=GOTO(13),[N_alias_section]=GOTO(5),[N_production_section]=GOTO(4),[N_extern_section]=GOTO(3),},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{[N_path_list_PRIME]=GOTO(30),},
/*  17 */	{},
/*  18 */	{[N_production]=GOTO(28),[N_production_list_PRIME]=GOTO(27),},
/*  19 */	{[N_follow_prod]=GOTO(24),},
/*  20 */	{},
/*  21 */	{},
/*  22 */	{},
/*  23 */	{},
/*  24 */	{},
/*  25 */	{},
/*  26 */	{[N_symbol]=GOTO(41),[N_symbol_list]=GOTO(39),[N_opt_list]=GOTO(38),[N_lhs]=GOTO(37),},
/*  27 */	{},
/*  28 */	{[N_production_list_PRIME]=GOTO(36),[N_production]=GOTO(28),},
/*  29 */	{},
/*  30 */	{},
/*  31 */	{[N_path_list]=GOTO(34),},
/*  32 */	{},
/*  33 */	{},
/*  34 */	{},
/*  35 */	{},
/*  36 */	{},
/*  37 */	{},
/*  38 */	{[N_lhs_PRIME]=GOTO(52),},
/*  39 */	{},
/*  40 */	{},
/*  41 */	{[N_symbol]=GOTO(51),[N_symbol_list_PRIME]=GOTO(50),},
/*  42 */	{},
/*  43 */	{},
/*  44 */	{},
/*  45 */	{},
/*  46 */	{},
/*  47 */	{},
/*  48 */	{},
/*  49 */	{},
/*  50 */	{},
/*  51 */	{[N_symbol_list_PRIME]=GOTO(55),[N_symbol]=GOTO(51),},
/*  52 */	{},
/*  53 */	{[N_opt_list]=GOTO(54),[N_symbol]=GOTO(41),[N_symbol_list]=GOTO(39),},
/*  54 */	{[N_lhs_PRIME]=GOTO(59),},
/*  55 */	{},
/*  56 */	{[N_alias_list_PRIME]=GOTO(57),},
/*  57 */	{},
/*  58 */	{[N_alias_list]=GOTO(60),},
/*  59 */	{},
/*  60 */	{},
};

#endif
static int
fgfp_sync_token[TOTAL_NTER][6] = {
	[N_FGFP] = { T_EOF, -1 },
	[N_section] = { T_EXTERN, T_ALIAS, T_PRODUCTION, T_EOF, -1 },
	[N_FGFP_PRIME] = { T_EOF, -1 },
	[N_extern_section] = { T_EXTERN, T_ALIAS, T_PRODUCTION, T_EOF, -1 },
	[N_production_section] = { T_EXTERN, T_ALIAS, T_PRODUCTION, T_EOF, -1 },
	[N_alias_section] = { T_EXTERN, T_ALIAS, T_PRODUCTION, T_EOF, -1 },
	[N_path_list] = { T_RBRACE, -1 },
	[N_path_list_PRIME] = { T_RBRACE, -1 },
	[N_alias_list] = { T_RBRACE, -1 },
	[N_alias_list_PRIME] = { T_RBRACE, -1 },
	[N_production_list] = { T_RBRACE, -1 },
	[N_production] = { T_NTER, T_RBRACE, -1 },
	[N_production_list_PRIME] = { T_RBRACE, -1 },
	[N_follow_prod] = { T_SEMI, -1 },
	[N_lhs] = { T_SEMI, -1 },
	[N_opt_list] = { T_SEMI, T_UNION, -1 },
	[N_lhs_PRIME] = { T_SEMI, -1 },
	[N_symbol_list] = { T_SEMI, T_UNION, -1 },
	[N_symbol] = { T_LITERAL, T_GLOBAL_TOK, T_NTER, T_SEMI, T_UNION, -1 },
	[N_symbol_list_PRIME] = { T_SEMI, T_UNION, -1 },
};

#endif /* _FGFP_GRM_H */
