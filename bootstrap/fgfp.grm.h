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

#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFP

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
