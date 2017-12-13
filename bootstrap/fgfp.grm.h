#ifndef _FGFP_GRM_H_H
#define _FGFP_GRM_H_H

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

static int
fgfp_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFP][T_PRODUCTION]=1, [N_FGFP][T_ALIAS]=1, [N_FGFP][T_EXTERN]=1, 
	[N_section][T_ALIAS]=6, [N_section][T_PRODUCTION]=5, [N_section][T_EXTERN]=4, 
	[N_FGFP_PRIME][T_PRODUCTION]=2, [N_FGFP_PRIME][T_ALIAS]=2, [N_FGFP_PRIME][T_EXTERN]=2, 
	[N_extern_section][T_EXTERN]=7, 
	[N_production_section][T_PRODUCTION]=17, 
	[N_alias_section][T_ALIAS]=12, 
	[N_path_list][T_RBRACE]=9, [N_path_list][T_LITERAL]=8, 
	[N_path_list_PRIME][T_RBRACE]=11, [N_path_list_PRIME][T_COMMA]=10, 
	[N_alias_list][T_RBRACE]=14, [N_alias_list][T_LPAREN]=13, 
	[N_alias_list_PRIME][T_RBRACE]=16, [N_alias_list_PRIME][T_COMMA]=15, 
	[N_production_list][T_RBRACE]=19, [N_production_list][T_NTER]=18, 
	[N_production][T_NTER]=22, 
	[N_production_list_PRIME][T_RBRACE]=21, [N_production_list_PRIME][T_NTER]=20, 
	[N_follow_prod][T_ARROW]=24, [N_follow_prod][T_EQUAL]=23, 
	[N_lhs][T_SEMI]=25, [N_lhs][T_EMPTY]=25, [N_lhs][T_UNION]=25, [N_lhs][T_NTER]=25, [N_lhs][T_GLOBAL_TOK]=25, [N_lhs][T_LITERAL]=25, 
	[N_opt_list][T_UNION]=30, [N_opt_list][T_SEMI]=30, [N_opt_list][T_EMPTY]=29, [N_opt_list][T_NTER]=28, [N_opt_list][T_GLOBAL_TOK]=28, [N_opt_list][T_LITERAL]=28, 
	[N_lhs_PRIME][T_SEMI]=27, [N_lhs_PRIME][T_UNION]=26, 
	[N_symbol_list][T_NTER]=31, [N_symbol_list][T_GLOBAL_TOK]=31, [N_symbol_list][T_LITERAL]=31, 
	[N_symbol][T_LITERAL]=36, [N_symbol][T_NTER]=35, [N_symbol][T_GLOBAL_TOK]=34, 
	[N_symbol_list_PRIME][T_UNION]=33, [N_symbol_list_PRIME][T_SEMI]=33, [N_symbol_list_PRIME][T_NTER]=32, [N_symbol_list_PRIME][T_GLOBAL_TOK]=32, [N_symbol_list_PRIME][T_LITERAL]=32, 
};

static int
fgfp_sync_token[TOTAL_NTER][6] = {
	[N_FGFP] = {-1},
	[N_section] = {T_EXTERN, T_ALIAS, T_PRODUCTION, -1},
	[N_FGFP_PRIME] = {-1},
	[N_extern_section] = {T_EXTERN, T_ALIAS, T_PRODUCTION, -1},
	[N_production_section] = {T_EXTERN, T_ALIAS, T_PRODUCTION, -1},
	[N_alias_section] = {T_EXTERN, T_ALIAS, T_PRODUCTION, -1},
	[N_path_list] = {T_RBRACE, -1},
	[N_path_list_PRIME] = {T_RBRACE, -1},
	[N_alias_list] = {T_RBRACE, -1},
	[N_alias_list_PRIME] = {T_RBRACE, -1},
	[N_production_list] = {T_RBRACE, -1},
	[N_production] = {T_NTER, T_RBRACE, -1},
	[N_production_list_PRIME] = {T_RBRACE, -1},
	[N_follow_prod] = {T_SEMI, -1},
	[N_lhs] = {T_SEMI, -1},
	[N_opt_list] = {T_SEMI, T_UNION, -1},
	[N_lhs_PRIME] = {T_SEMI, -1},
	[N_symbol_list] = {T_SEMI, T_UNION, -1},
	[N_symbol] = {T_LITERAL, T_GLOBAL_TOK, T_NTER, T_SEMI, T_UNION, -1},
	[N_symbol_list_PRIME] = {T_SEMI, T_UNION, -1},
};

#endif /* _FGFP_GRM_H_H */
