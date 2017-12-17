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
#define P_N_FGFP_N_section_N_FGFP_PRIME	1
#define P_N_FGFP_PRIME_N_section_N_FGFP_PRIME	2
#define P_N_FGFP_PRIME	3
#define P_N_section_N_extern_section	4
#define P_N_section_N_production_section	5
#define P_N_section_N_alias_section	6
#define P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI	7
#define P_N_path_list_T_LITERAL_N_path_list_PRIME	8
#define P_N_path_list	9
#define P_N_path_list_PRIME_T_COMMA_N_path_list	10
#define P_N_path_list_PRIME	11
#define P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI	12
#define P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME	13
#define P_N_alias_list	14
#define P_N_alias_list_PRIME_T_COMMA_N_alias_list	15
#define P_N_alias_list_PRIME	16
#define P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI	17
#define P_N_production_list_N_production_N_production_list_PRIME	18
#define P_N_production_list	19
#define P_N_production_list_PRIME_N_production_N_production_list_PRIME	20
#define P_N_production_list_PRIME	21
#define P_N_production_T_NTER_N_follow_prod_T_SEMI	22
#define P_N_follow_prod_T_EQUAL_T_START	23
#define P_N_follow_prod_T_ARROW_N_lhs	24
#define P_N_lhs_N_opt_list_N_lhs_PRIME	25
#define P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME	26
#define P_N_lhs_PRIME	27
#define P_N_opt_list_N_symbol_list	28
#define P_N_opt_list_T_EMPTY	29
#define P_N_opt_list	30
#define P_N_symbol_list_N_symbol_N_symbol_list_PRIME	31
#define P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME	32
#define P_N_symbol_list_PRIME	33
#define P_N_symbol_T_GLOBAL_TOK	34
#define P_N_symbol_T_NTER	35
#define P_N_symbol_T_LITERAL	36


#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFP

static int
fgfp_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFP][T_PRODUCTION]=P_N_FGFP_N_section_N_FGFP_PRIME, [N_FGFP][T_ALIAS]=P_N_FGFP_N_section_N_FGFP_PRIME, [N_FGFP][T_EXTERN]=P_N_FGFP_N_section_N_FGFP_PRIME, 
	[N_section][T_ALIAS]=P_N_section_N_alias_section, [N_section][T_PRODUCTION]=P_N_section_N_production_section, [N_section][T_EXTERN]=P_N_section_N_extern_section, 
	[N_FGFP_PRIME][T_EOF]=P_N_FGFP_PRIME, [N_FGFP_PRIME][T_PRODUCTION]=P_N_FGFP_PRIME_N_section_N_FGFP_PRIME, [N_FGFP_PRIME][T_ALIAS]=P_N_FGFP_PRIME_N_section_N_FGFP_PRIME, [N_FGFP_PRIME][T_EXTERN]=P_N_FGFP_PRIME_N_section_N_FGFP_PRIME, 
	[N_extern_section][T_EXTERN]=P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI, 
	[N_production_section][T_PRODUCTION]=P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI, 
	[N_alias_section][T_ALIAS]=P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI, 
	[N_path_list][T_RBRACE]=P_N_path_list, [N_path_list][T_LITERAL]=P_N_path_list_T_LITERAL_N_path_list_PRIME, 
	[N_path_list_PRIME][T_RBRACE]=P_N_path_list_PRIME, [N_path_list_PRIME][T_COMMA]=P_N_path_list_PRIME_T_COMMA_N_path_list, 
	[N_alias_list][T_RBRACE]=P_N_alias_list, [N_alias_list][T_LPAREN]=P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME, 
	[N_alias_list_PRIME][T_RBRACE]=P_N_alias_list_PRIME, [N_alias_list_PRIME][T_COMMA]=P_N_alias_list_PRIME_T_COMMA_N_alias_list, 
	[N_production_list][T_RBRACE]=P_N_production_list, [N_production_list][T_NTER]=P_N_production_list_N_production_N_production_list_PRIME, 
	[N_production][T_NTER]=P_N_production_T_NTER_N_follow_prod_T_SEMI, 
	[N_production_list_PRIME][T_RBRACE]=P_N_production_list_PRIME, [N_production_list_PRIME][T_NTER]=P_N_production_list_PRIME_N_production_N_production_list_PRIME, 
	[N_follow_prod][T_ARROW]=P_N_follow_prod_T_ARROW_N_lhs, [N_follow_prod][T_EQUAL]=P_N_follow_prod_T_EQUAL_T_START, 
	[N_lhs][T_SEMI]=P_N_lhs_N_opt_list_N_lhs_PRIME, [N_lhs][T_EMPTY]=P_N_lhs_N_opt_list_N_lhs_PRIME, [N_lhs][T_UNION]=P_N_lhs_N_opt_list_N_lhs_PRIME, [N_lhs][T_NTER]=P_N_lhs_N_opt_list_N_lhs_PRIME, [N_lhs][T_GLOBAL_TOK]=P_N_lhs_N_opt_list_N_lhs_PRIME, [N_lhs][T_LITERAL]=P_N_lhs_N_opt_list_N_lhs_PRIME, 
	[N_opt_list][T_UNION]=P_N_opt_list, [N_opt_list][T_SEMI]=P_N_opt_list, [N_opt_list][T_EMPTY]=P_N_opt_list_T_EMPTY, [N_opt_list][T_NTER]=P_N_opt_list_N_symbol_list, [N_opt_list][T_GLOBAL_TOK]=P_N_opt_list_N_symbol_list, [N_opt_list][T_LITERAL]=P_N_opt_list_N_symbol_list, 
	[N_lhs_PRIME][T_SEMI]=P_N_lhs_PRIME, [N_lhs_PRIME][T_UNION]=P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME, 
	[N_symbol_list][T_NTER]=P_N_symbol_list_N_symbol_N_symbol_list_PRIME, [N_symbol_list][T_GLOBAL_TOK]=P_N_symbol_list_N_symbol_N_symbol_list_PRIME, [N_symbol_list][T_LITERAL]=P_N_symbol_list_N_symbol_N_symbol_list_PRIME, 
	[N_symbol][T_LITERAL]=P_N_symbol_T_LITERAL, [N_symbol][T_NTER]=P_N_symbol_T_NTER, [N_symbol][T_GLOBAL_TOK]=P_N_symbol_T_GLOBAL_TOK, 
	[N_symbol_list_PRIME][T_UNION]=P_N_symbol_list_PRIME, [N_symbol_list_PRIME][T_SEMI]=P_N_symbol_list_PRIME, [N_symbol_list_PRIME][T_NTER]=P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME, [N_symbol_list_PRIME][T_GLOBAL_TOK]=P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME, [N_symbol_list_PRIME][T_LITERAL]=P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME, 
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

static int
lr_action_table[61][TOTAL_TOKEN] = {
/*   0 */	{[T_ALIAS]=SHIFT(8),[T_PRODUCTION]=SHIFT(7),[T_EXTERN]=SHIFT(6),},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[T_ALIAS]=SHIFT(8),[T_PRODUCTION]=SHIFT(7),[T_EXTERN]=SHIFT(6),},
/*   3 */	{[T_EOF]=REDUCE(P_N_section_N_extern_section),[T_BARROW]=REDUCE(P_N_section_N_extern_section),[T_ARROW]=REDUCE(P_N_section_N_extern_section),[T_EMPTY]=REDUCE(P_N_section_N_extern_section),[T_UNION]=REDUCE(P_N_section_N_extern_section),[T_EQUAL]=REDUCE(P_N_section_N_extern_section),[T_RBRACE]=REDUCE(P_N_section_N_extern_section),[T_LBRACE]=REDUCE(P_N_section_N_extern_section),[T_RPAREN]=REDUCE(P_N_section_N_extern_section),[T_LPAREN]=REDUCE(P_N_section_N_extern_section),[T_SEMI]=REDUCE(P_N_section_N_extern_section),[T_COMMA]=REDUCE(P_N_section_N_extern_section),[T_START]=REDUCE(P_N_section_N_extern_section),[T_PRODUCTION]=REDUCE(P_N_section_N_extern_section),[T_ALIAS]=REDUCE(P_N_section_N_extern_section),[T_EXTERN]=REDUCE(P_N_section_N_extern_section),[T_NTER]=REDUCE(P_N_section_N_extern_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_extern_section),[T_LITERAL]=REDUCE(P_N_section_N_extern_section),},
/*   4 */	{[T_EOF]=REDUCE(P_N_section_N_production_section),[T_BARROW]=REDUCE(P_N_section_N_production_section),[T_ARROW]=REDUCE(P_N_section_N_production_section),[T_EMPTY]=REDUCE(P_N_section_N_production_section),[T_UNION]=REDUCE(P_N_section_N_production_section),[T_EQUAL]=REDUCE(P_N_section_N_production_section),[T_RBRACE]=REDUCE(P_N_section_N_production_section),[T_LBRACE]=REDUCE(P_N_section_N_production_section),[T_RPAREN]=REDUCE(P_N_section_N_production_section),[T_LPAREN]=REDUCE(P_N_section_N_production_section),[T_SEMI]=REDUCE(P_N_section_N_production_section),[T_COMMA]=REDUCE(P_N_section_N_production_section),[T_START]=REDUCE(P_N_section_N_production_section),[T_PRODUCTION]=REDUCE(P_N_section_N_production_section),[T_ALIAS]=REDUCE(P_N_section_N_production_section),[T_EXTERN]=REDUCE(P_N_section_N_production_section),[T_NTER]=REDUCE(P_N_section_N_production_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_production_section),[T_LITERAL]=REDUCE(P_N_section_N_production_section),},
/*   5 */	{[T_EOF]=REDUCE(P_N_section_N_alias_section),[T_BARROW]=REDUCE(P_N_section_N_alias_section),[T_ARROW]=REDUCE(P_N_section_N_alias_section),[T_EMPTY]=REDUCE(P_N_section_N_alias_section),[T_UNION]=REDUCE(P_N_section_N_alias_section),[T_EQUAL]=REDUCE(P_N_section_N_alias_section),[T_RBRACE]=REDUCE(P_N_section_N_alias_section),[T_LBRACE]=REDUCE(P_N_section_N_alias_section),[T_RPAREN]=REDUCE(P_N_section_N_alias_section),[T_LPAREN]=REDUCE(P_N_section_N_alias_section),[T_SEMI]=REDUCE(P_N_section_N_alias_section),[T_COMMA]=REDUCE(P_N_section_N_alias_section),[T_START]=REDUCE(P_N_section_N_alias_section),[T_PRODUCTION]=REDUCE(P_N_section_N_alias_section),[T_ALIAS]=REDUCE(P_N_section_N_alias_section),[T_EXTERN]=REDUCE(P_N_section_N_alias_section),[T_NTER]=REDUCE(P_N_section_N_alias_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_alias_section),[T_LITERAL]=REDUCE(P_N_section_N_alias_section),},
/*   6 */	{[T_LBRACE]=SHIFT(11),},
/*   7 */	{[T_LBRACE]=SHIFT(10),},
/*   8 */	{[T_LBRACE]=SHIFT(9),},
/*   9 */	{[T_LPAREN]=SHIFT(21),},
/*  10 */	{[T_NTER]=SHIFT(19),},
/*  11 */	{[T_LITERAL]=SHIFT(16),},
/*  12 */	{[T_EOF]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_BARROW]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_ARROW]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_EMPTY]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_UNION]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_EQUAL]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_RBRACE]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_LBRACE]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_RPAREN]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_LPAREN]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_SEMI]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_COMMA]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_START]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_PRODUCTION]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_ALIAS]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_EXTERN]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_NTER]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),[T_LITERAL]=REDUCE(P_N_FGFP_N_section_N_FGFP_PRIME),},
/*  13 */	{[T_ALIAS]=SHIFT(8),[T_PRODUCTION]=SHIFT(7),[T_EXTERN]=SHIFT(6),},
/*  14 */	{[T_EOF]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_BARROW]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_ARROW]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_EMPTY]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_UNION]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_EQUAL]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_RBRACE]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_LBRACE]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_RPAREN]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_LPAREN]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_SEMI]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_COMMA]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_START]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_PRODUCTION]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_ALIAS]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_EXTERN]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_NTER]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),[T_LITERAL]=REDUCE(P_N_FGFP_PRIME_N_section_N_FGFP_PRIME),},
/*  15 */	{[T_RBRACE]=SHIFT(32),},
/*  16 */	{[T_COMMA]=SHIFT(31),},
/*  17 */	{[T_RBRACE]=SHIFT(29),},
/*  18 */	{[T_NTER]=SHIFT(19),},
/*  19 */	{[T_ARROW]=SHIFT(26),[T_EQUAL]=SHIFT(25),},
/*  20 */	{[T_RBRACE]=SHIFT(23),},
/*  21 */	{[T_GLOBAL_TOK]=SHIFT(22),},
/*  22 */	{[T_BARROW]=SHIFT(48),},
/*  23 */	{[T_SEMI]=SHIFT(47),},
/*  24 */	{[T_SEMI]=SHIFT(46),},
/*  25 */	{[T_START]=SHIFT(45),},
/*  26 */	{[T_LITERAL]=SHIFT(44),[T_NTER]=SHIFT(43),[T_GLOBAL_TOK]=SHIFT(42),[T_EMPTY]=SHIFT(40),},
/*  27 */	{[T_EOF]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_BARROW]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_ARROW]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_EMPTY]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_UNION]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_EQUAL]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_RBRACE]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_LBRACE]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_RPAREN]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_LPAREN]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_SEMI]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_COMMA]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_START]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_ALIAS]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_EXTERN]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_NTER]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),[T_LITERAL]=REDUCE(P_N_production_list_N_production_N_production_list_PRIME),},
/*  28 */	{[T_NTER]=SHIFT(19),},
/*  29 */	{[T_SEMI]=SHIFT(35),},
/*  30 */	{[T_EOF]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_BARROW]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_ARROW]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_EMPTY]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_UNION]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_EQUAL]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_RBRACE]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_LBRACE]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_RPAREN]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_LPAREN]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_SEMI]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_COMMA]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_START]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_ALIAS]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_EXTERN]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_NTER]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),[T_LITERAL]=REDUCE(P_N_path_list_T_LITERAL_N_path_list_PRIME),},
/*  31 */	{[T_LITERAL]=SHIFT(16),},
/*  32 */	{[T_SEMI]=SHIFT(33),},
/*  33 */	{[T_EOF]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_BARROW]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_EMPTY]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_UNION]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_RPAREN]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_LPAREN]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_START]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_PRODUCTION]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_ALIAS]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_EXTERN]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_NTER]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),[T_LITERAL]=REDUCE(P_N_extern_section_T_EXTERN_T_LBRACE_N_path_list_T_RBRACE_T_SEMI),},
/*  34 */	{[T_EOF]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_BARROW]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_ARROW]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_EMPTY]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_UNION]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_EQUAL]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_RBRACE]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_LBRACE]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_RPAREN]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_LPAREN]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_SEMI]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_COMMA]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_START]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_PRODUCTION]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_ALIAS]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_EXTERN]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_NTER]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_GLOBAL_TOK]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),[T_LITERAL]=REDUCE(P_N_path_list_PRIME_T_COMMA_N_path_list),},
/*  35 */	{[T_EOF]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_BARROW]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_EMPTY]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_UNION]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_RPAREN]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_LPAREN]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_START]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_PRODUCTION]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_ALIAS]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_EXTERN]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_NTER]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),[T_LITERAL]=REDUCE(P_N_production_section_T_PRODUCTION_T_LBRACE_N_production_list_T_RBRACE_T_SEMI),},
/*  36 */	{[T_EOF]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_BARROW]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_ARROW]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_EMPTY]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_UNION]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_EQUAL]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_RBRACE]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_LBRACE]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_RPAREN]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_LPAREN]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_SEMI]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_COMMA]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_START]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_ALIAS]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_EXTERN]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_NTER]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),[T_LITERAL]=REDUCE(P_N_production_list_PRIME_N_production_N_production_list_PRIME),},
/*  37 */	{[T_EOF]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_BARROW]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_ARROW]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_EMPTY]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_UNION]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_EQUAL]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_RBRACE]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_LBRACE]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_RPAREN]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_LPAREN]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_SEMI]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_COMMA]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_START]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_PRODUCTION]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_ALIAS]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_EXTERN]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_NTER]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_GLOBAL_TOK]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),[T_LITERAL]=REDUCE(P_N_follow_prod_T_ARROW_N_lhs),},
/*  38 */	{[T_UNION]=SHIFT(53),},
/*  39 */	{[T_EOF]=REDUCE(P_N_opt_list_N_symbol_list),[T_BARROW]=REDUCE(P_N_opt_list_N_symbol_list),[T_ARROW]=REDUCE(P_N_opt_list_N_symbol_list),[T_EMPTY]=REDUCE(P_N_opt_list_N_symbol_list),[T_UNION]=REDUCE(P_N_opt_list_N_symbol_list),[T_EQUAL]=REDUCE(P_N_opt_list_N_symbol_list),[T_RBRACE]=REDUCE(P_N_opt_list_N_symbol_list),[T_LBRACE]=REDUCE(P_N_opt_list_N_symbol_list),[T_RPAREN]=REDUCE(P_N_opt_list_N_symbol_list),[T_LPAREN]=REDUCE(P_N_opt_list_N_symbol_list),[T_SEMI]=REDUCE(P_N_opt_list_N_symbol_list),[T_COMMA]=REDUCE(P_N_opt_list_N_symbol_list),[T_START]=REDUCE(P_N_opt_list_N_symbol_list),[T_PRODUCTION]=REDUCE(P_N_opt_list_N_symbol_list),[T_ALIAS]=REDUCE(P_N_opt_list_N_symbol_list),[T_EXTERN]=REDUCE(P_N_opt_list_N_symbol_list),[T_NTER]=REDUCE(P_N_opt_list_N_symbol_list),[T_GLOBAL_TOK]=REDUCE(P_N_opt_list_N_symbol_list),[T_LITERAL]=REDUCE(P_N_opt_list_N_symbol_list),},
/*  40 */	{[T_EOF]=REDUCE(P_N_opt_list_T_EMPTY),[T_BARROW]=REDUCE(P_N_opt_list_T_EMPTY),[T_ARROW]=REDUCE(P_N_opt_list_T_EMPTY),[T_EMPTY]=REDUCE(P_N_opt_list_T_EMPTY),[T_UNION]=REDUCE(P_N_opt_list_T_EMPTY),[T_EQUAL]=REDUCE(P_N_opt_list_T_EMPTY),[T_RBRACE]=REDUCE(P_N_opt_list_T_EMPTY),[T_LBRACE]=REDUCE(P_N_opt_list_T_EMPTY),[T_RPAREN]=REDUCE(P_N_opt_list_T_EMPTY),[T_LPAREN]=REDUCE(P_N_opt_list_T_EMPTY),[T_SEMI]=REDUCE(P_N_opt_list_T_EMPTY),[T_COMMA]=REDUCE(P_N_opt_list_T_EMPTY),[T_START]=REDUCE(P_N_opt_list_T_EMPTY),[T_PRODUCTION]=REDUCE(P_N_opt_list_T_EMPTY),[T_ALIAS]=REDUCE(P_N_opt_list_T_EMPTY),[T_EXTERN]=REDUCE(P_N_opt_list_T_EMPTY),[T_NTER]=REDUCE(P_N_opt_list_T_EMPTY),[T_GLOBAL_TOK]=REDUCE(P_N_opt_list_T_EMPTY),[T_LITERAL]=REDUCE(P_N_opt_list_T_EMPTY),},
/*  41 */	{[T_LITERAL]=SHIFT(44),[T_NTER]=SHIFT(43),[T_GLOBAL_TOK]=SHIFT(42),},
/*  42 */	{[T_EOF]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_BARROW]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_ARROW]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_EMPTY]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_UNION]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_EQUAL]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_RBRACE]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_LBRACE]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_RPAREN]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_LPAREN]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_SEMI]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_COMMA]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_START]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_PRODUCTION]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_ALIAS]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_EXTERN]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_NTER]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_GLOBAL_TOK]=REDUCE(P_N_symbol_T_GLOBAL_TOK),[T_LITERAL]=REDUCE(P_N_symbol_T_GLOBAL_TOK),},
/*  43 */	{[T_EOF]=REDUCE(P_N_symbol_T_NTER),[T_BARROW]=REDUCE(P_N_symbol_T_NTER),[T_ARROW]=REDUCE(P_N_symbol_T_NTER),[T_EMPTY]=REDUCE(P_N_symbol_T_NTER),[T_UNION]=REDUCE(P_N_symbol_T_NTER),[T_EQUAL]=REDUCE(P_N_symbol_T_NTER),[T_RBRACE]=REDUCE(P_N_symbol_T_NTER),[T_LBRACE]=REDUCE(P_N_symbol_T_NTER),[T_RPAREN]=REDUCE(P_N_symbol_T_NTER),[T_LPAREN]=REDUCE(P_N_symbol_T_NTER),[T_SEMI]=REDUCE(P_N_symbol_T_NTER),[T_COMMA]=REDUCE(P_N_symbol_T_NTER),[T_START]=REDUCE(P_N_symbol_T_NTER),[T_PRODUCTION]=REDUCE(P_N_symbol_T_NTER),[T_ALIAS]=REDUCE(P_N_symbol_T_NTER),[T_EXTERN]=REDUCE(P_N_symbol_T_NTER),[T_NTER]=REDUCE(P_N_symbol_T_NTER),[T_GLOBAL_TOK]=REDUCE(P_N_symbol_T_NTER),[T_LITERAL]=REDUCE(P_N_symbol_T_NTER),},
/*  44 */	{[T_EOF]=REDUCE(P_N_symbol_T_LITERAL),[T_BARROW]=REDUCE(P_N_symbol_T_LITERAL),[T_ARROW]=REDUCE(P_N_symbol_T_LITERAL),[T_EMPTY]=REDUCE(P_N_symbol_T_LITERAL),[T_UNION]=REDUCE(P_N_symbol_T_LITERAL),[T_EQUAL]=REDUCE(P_N_symbol_T_LITERAL),[T_RBRACE]=REDUCE(P_N_symbol_T_LITERAL),[T_LBRACE]=REDUCE(P_N_symbol_T_LITERAL),[T_RPAREN]=REDUCE(P_N_symbol_T_LITERAL),[T_LPAREN]=REDUCE(P_N_symbol_T_LITERAL),[T_SEMI]=REDUCE(P_N_symbol_T_LITERAL),[T_COMMA]=REDUCE(P_N_symbol_T_LITERAL),[T_START]=REDUCE(P_N_symbol_T_LITERAL),[T_PRODUCTION]=REDUCE(P_N_symbol_T_LITERAL),[T_ALIAS]=REDUCE(P_N_symbol_T_LITERAL),[T_EXTERN]=REDUCE(P_N_symbol_T_LITERAL),[T_NTER]=REDUCE(P_N_symbol_T_LITERAL),[T_GLOBAL_TOK]=REDUCE(P_N_symbol_T_LITERAL),[T_LITERAL]=REDUCE(P_N_symbol_T_LITERAL),},
/*  45 */	{[T_EOF]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_BARROW]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_ARROW]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_EMPTY]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_UNION]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_EQUAL]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_RBRACE]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_LBRACE]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_RPAREN]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_LPAREN]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_SEMI]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_COMMA]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_START]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_PRODUCTION]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_ALIAS]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_EXTERN]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_NTER]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_GLOBAL_TOK]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),[T_LITERAL]=REDUCE(P_N_follow_prod_T_EQUAL_T_START),},
/*  46 */	{[T_EOF]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_BARROW]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_ARROW]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_EMPTY]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_UNION]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_EQUAL]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_RBRACE]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_LBRACE]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_RPAREN]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_LPAREN]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_SEMI]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_COMMA]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_START]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_PRODUCTION]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_ALIAS]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_EXTERN]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_NTER]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),[T_LITERAL]=REDUCE(P_N_production_T_NTER_N_follow_prod_T_SEMI),},
/*  47 */	{[T_EOF]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_BARROW]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_EMPTY]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_UNION]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_RPAREN]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_LPAREN]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_START]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_PRODUCTION]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_ALIAS]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_EXTERN]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_NTER]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),[T_LITERAL]=REDUCE(P_N_alias_section_T_ALIAS_T_LBRACE_N_alias_list_T_RBRACE_T_SEMI),},
/*  48 */	{[T_LITERAL]=SHIFT(49),},
/*  49 */	{[T_RPAREN]=SHIFT(56),},
/*  50 */	{[T_EOF]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_BARROW]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_ARROW]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_EMPTY]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_UNION]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_EQUAL]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_RBRACE]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_LBRACE]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_RPAREN]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_LPAREN]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_SEMI]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_COMMA]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_START]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_ALIAS]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_EXTERN]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_NTER]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),[T_LITERAL]=REDUCE(P_N_symbol_list_N_symbol_N_symbol_list_PRIME),},
/*  51 */	{[T_LITERAL]=SHIFT(44),[T_NTER]=SHIFT(43),[T_GLOBAL_TOK]=SHIFT(42),},
/*  52 */	{[T_EOF]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_BARROW]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_ARROW]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_EMPTY]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_UNION]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_EQUAL]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_RBRACE]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_LBRACE]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_RPAREN]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_LPAREN]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_SEMI]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_COMMA]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_START]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_PRODUCTION]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_ALIAS]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_EXTERN]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_NTER]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),[T_LITERAL]=REDUCE(P_N_lhs_N_opt_list_N_lhs_PRIME),},
/*  53 */	{[T_LITERAL]=SHIFT(44),[T_NTER]=SHIFT(43),[T_GLOBAL_TOK]=SHIFT(42),[T_EMPTY]=SHIFT(40),},
/*  54 */	{[T_UNION]=SHIFT(53),},
/*  55 */	{[T_EOF]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_BARROW]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_ARROW]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_EMPTY]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_UNION]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_EQUAL]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_RBRACE]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_LBRACE]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_RPAREN]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_LPAREN]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_SEMI]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_COMMA]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_START]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_ALIAS]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_EXTERN]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_NTER]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),[T_LITERAL]=REDUCE(P_N_symbol_list_PRIME_N_symbol_N_symbol_list_PRIME),},
/*  56 */	{[T_COMMA]=SHIFT(58),},
/*  57 */	{[T_EOF]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_BARROW]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_ARROW]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_EMPTY]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_UNION]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_EQUAL]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_RBRACE]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_LBRACE]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_RPAREN]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_LPAREN]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_SEMI]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_COMMA]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_START]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_PRODUCTION]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_ALIAS]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_EXTERN]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_NTER]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),[T_LITERAL]=REDUCE(P_N_alias_list_T_LPAREN_T_GLOBAL_TOK_T_BARROW_T_LITERAL_T_RPAREN_N_alias_list_PRIME),},
/*  58 */	{[T_LPAREN]=SHIFT(21),},
/*  59 */	{[T_EOF]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_BARROW]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_ARROW]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_EMPTY]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_UNION]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_EQUAL]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_RBRACE]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_LBRACE]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_RPAREN]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_LPAREN]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_SEMI]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_COMMA]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_START]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_PRODUCTION]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_ALIAS]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_EXTERN]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_NTER]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),[T_LITERAL]=REDUCE(P_N_lhs_PRIME_T_UNION_N_opt_list_N_lhs_PRIME),},
/*  60 */	{[T_EOF]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_BARROW]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_ARROW]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_EMPTY]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_UNION]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_EQUAL]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_RBRACE]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_LBRACE]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_RPAREN]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_LPAREN]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_SEMI]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_COMMA]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_START]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_PRODUCTION]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_ALIAS]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_EXTERN]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_NTER]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_GLOBAL_TOK]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),[T_LITERAL]=REDUCE(P_N_alias_list_PRIME_T_COMMA_N_alias_list),},
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
