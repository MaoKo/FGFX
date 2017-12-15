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

#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

//Various macro representing production.
#define P_N_FGFL_N_section_N_FGFL_PRIME	1
#define P_N_FGFL_PRIME_N_section_N_FGFL_PRIME	2
#define P_N_FGFL_PRIME	3
#define P_N_section_N_skip_section	4
#define P_N_section_N_token_section	5
#define P_N_section_N_keyword_section	6
#define P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI	7
#define P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI	8
#define P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI	9
#define P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list	10
#define P_N_regex_list	11
#define P_N_regex_assign_T_EQUAL_T_REGEX	12
#define P_N_regex_assign_T_ARROW_T_IGCASE	13
#define P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME	14
#define P_N_keyword_list	15
#define P_N_keyword_list_PRIME_T_COMMA_N_keyword_list	16
#define P_N_keyword_list_PRIME	17
#define P_N_atom_FGFL_T_GLOBAL_TOK	18
#define P_N_atom_FGFL_T_LOCAL_TOK	19

static int
fgfl_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_FGFL][T_KEYWORD]=P_N_FGFL_N_section_N_FGFL_PRIME, [N_FGFL][T_TOKEN]=P_N_FGFL_N_section_N_FGFL_PRIME, [N_FGFL][T_SKIP]=P_N_FGFL_N_section_N_FGFL_PRIME, 
	[N_section][T_KEYWORD]=P_N_section_N_keyword_section, [N_section][T_TOKEN]=P_N_section_N_token_section, [N_section][T_SKIP]=P_N_section_N_skip_section, 
	[N_FGFL_PRIME][T_EOF]=P_N_FGFL_PRIME, [N_FGFL_PRIME][T_KEYWORD]=P_N_FGFL_PRIME_N_section_N_FGFL_PRIME, [N_FGFL_PRIME][T_TOKEN]=P_N_FGFL_PRIME_N_section_N_FGFL_PRIME, [N_FGFL_PRIME][T_SKIP]=P_N_FGFL_PRIME_N_section_N_FGFL_PRIME, 
	[N_skip_section][T_SKIP]=P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI, 
	[N_token_section][T_TOKEN]=P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI, 
	[N_keyword_section][T_KEYWORD]=P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI, 
	[N_regex_list][T_RBRACE]=P_N_regex_list, [N_regex_list][T_LOCAL_TOK]=P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list, [N_regex_list][T_GLOBAL_TOK]=P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list, 
	[N_keyword_list][T_RBRACE]=P_N_keyword_list, [N_keyword_list][T_LOCAL_TOK]=P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME, [N_keyword_list][T_GLOBAL_TOK]=P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME, 
	[N_atom_FGFL][T_LOCAL_TOK]=P_N_atom_FGFL_T_LOCAL_TOK, [N_atom_FGFL][T_GLOBAL_TOK]=P_N_atom_FGFL_T_GLOBAL_TOK, 
	[N_regex_assign][T_ARROW]=P_N_regex_assign_T_ARROW_T_IGCASE, [N_regex_assign][T_EQUAL]=P_N_regex_assign_T_EQUAL_T_REGEX, 
	[N_keyword_list_PRIME][T_RBRACE]=P_N_keyword_list_PRIME, [N_keyword_list_PRIME][T_COMMA]=P_N_keyword_list_PRIME_T_COMMA_N_keyword_list, 
};

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
