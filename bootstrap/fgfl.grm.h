#ifndef _FGFL_GRM_H_H
#define _FGFL_GRM_H_H

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

#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

static int
fgfl_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFL][T_KEYWORD]=1, [N_FGFL][T_TOKEN]=1, [N_FGFL][T_SKIP]=1, 
	[N_section][T_KEYWORD]=6, [N_section][T_TOKEN]=5, [N_section][T_SKIP]=4, 
	[N_FGFL_PRIME][T_KEYWORD]=2, [N_FGFL_PRIME][T_TOKEN]=2, [N_FGFL_PRIME][T_SKIP]=2, 
	[N_skip_section][T_SKIP]=7, 
	[N_token_section][T_TOKEN]=8, 
	[N_keyword_section][T_KEYWORD]=9, 
	[N_regex_list][T_RBRACE]=11, [N_regex_list][T_LOCAL_TOK]=10, [N_regex_list][T_GLOBAL_TOK]=10, 
	[N_keyword_list][T_RBRACE]=15, [N_keyword_list][T_LOCAL_TOK]=14, [N_keyword_list][T_GLOBAL_TOK]=14, 
	[N_atom_FGFL][T_LOCAL_TOK]=19, [N_atom_FGFL][T_GLOBAL_TOK]=18, 
	[N_regex_assign][T_ARROW]=13, [N_regex_assign][T_EQUAL]=12, 
	[N_keyword_list_PRIME][T_RBRACE]=17, [N_keyword_list_PRIME][T_COMMA]=16, 
};

static int
fgfl_sync_token[TOTAL_NTER][5] = {
	[N_FGFL] = {-1},
	[N_section] = {T_SKIP, T_TOKEN, T_KEYWORD, -1},
	[N_FGFL_PRIME] = {-1},
	[N_skip_section] = {T_SKIP, T_TOKEN, T_KEYWORD, -1},
	[N_token_section] = {T_SKIP, T_TOKEN, T_KEYWORD, -1},
	[N_keyword_section] = {T_SKIP, T_TOKEN, T_KEYWORD, -1},
	[N_regex_list] = {T_RBRACE, -1},
	[N_keyword_list] = {T_RBRACE, -1},
	[N_atom_FGFL] = {T_RBRACE, T_COMMA, T_EQUAL, T_ARROW, -1},
	[N_regex_assign] = {T_SEMI, -1},
	[N_keyword_list_PRIME] = {T_RBRACE, -1},
};

#endif /* _FGFL_GRM_H_H */
