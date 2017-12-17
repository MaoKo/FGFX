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


#define ERROR_SLOT	0
#define START_SYMBOL	N_FGFL

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
lr_action_table[38][TOTAL_TOKEN] = {
/*   0 */	{[T_KEYWORD]=SHIFT(8),[T_TOKEN]=SHIFT(7),[T_SKIP]=SHIFT(6),},
/*   1 */	{[T_EOF]=ACCEPT,},
/*   2 */	{[T_KEYWORD]=SHIFT(8),[T_TOKEN]=SHIFT(7),[T_SKIP]=SHIFT(6),},
/*   3 */	{[T_EOF]=REDUCE(P_N_section_N_skip_section),[T_ARROW]=REDUCE(P_N_section_N_skip_section),[T_EQUAL]=REDUCE(P_N_section_N_skip_section),[T_COMMA]=REDUCE(P_N_section_N_skip_section),[T_SEMI]=REDUCE(P_N_section_N_skip_section),[T_RBRACE]=REDUCE(P_N_section_N_skip_section),[T_LBRACE]=REDUCE(P_N_section_N_skip_section),[T_IGCASE]=REDUCE(P_N_section_N_skip_section),[T_KEYWORD]=REDUCE(P_N_section_N_skip_section),[T_TOKEN]=REDUCE(P_N_section_N_skip_section),[T_SKIP]=REDUCE(P_N_section_N_skip_section),[T_LOCAL_TOK]=REDUCE(P_N_section_N_skip_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_skip_section),[T_REGEX]=REDUCE(P_N_section_N_skip_section),},
/*   4 */	{[T_EOF]=REDUCE(P_N_section_N_token_section),[T_ARROW]=REDUCE(P_N_section_N_token_section),[T_EQUAL]=REDUCE(P_N_section_N_token_section),[T_COMMA]=REDUCE(P_N_section_N_token_section),[T_SEMI]=REDUCE(P_N_section_N_token_section),[T_RBRACE]=REDUCE(P_N_section_N_token_section),[T_LBRACE]=REDUCE(P_N_section_N_token_section),[T_IGCASE]=REDUCE(P_N_section_N_token_section),[T_KEYWORD]=REDUCE(P_N_section_N_token_section),[T_TOKEN]=REDUCE(P_N_section_N_token_section),[T_SKIP]=REDUCE(P_N_section_N_token_section),[T_LOCAL_TOK]=REDUCE(P_N_section_N_token_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_token_section),[T_REGEX]=REDUCE(P_N_section_N_token_section),},
/*   5 */	{[T_EOF]=REDUCE(P_N_section_N_keyword_section),[T_ARROW]=REDUCE(P_N_section_N_keyword_section),[T_EQUAL]=REDUCE(P_N_section_N_keyword_section),[T_COMMA]=REDUCE(P_N_section_N_keyword_section),[T_SEMI]=REDUCE(P_N_section_N_keyword_section),[T_RBRACE]=REDUCE(P_N_section_N_keyword_section),[T_LBRACE]=REDUCE(P_N_section_N_keyword_section),[T_IGCASE]=REDUCE(P_N_section_N_keyword_section),[T_KEYWORD]=REDUCE(P_N_section_N_keyword_section),[T_TOKEN]=REDUCE(P_N_section_N_keyword_section),[T_SKIP]=REDUCE(P_N_section_N_keyword_section),[T_LOCAL_TOK]=REDUCE(P_N_section_N_keyword_section),[T_GLOBAL_TOK]=REDUCE(P_N_section_N_keyword_section),[T_REGEX]=REDUCE(P_N_section_N_keyword_section),},
/*   6 */	{[T_LBRACE]=SHIFT(11),},
/*   7 */	{[T_LBRACE]=SHIFT(10),},
/*   8 */	{[T_LBRACE]=SHIFT(9),},
/*   9 */	{[T_LOCAL_TOK]=SHIFT(16),[T_GLOBAL_TOK]=SHIFT(15),},
/*  10 */	{[T_LOCAL_TOK]=SHIFT(16),[T_GLOBAL_TOK]=SHIFT(15),},
/*  11 */	{[T_LOCAL_TOK]=SHIFT(16),[T_GLOBAL_TOK]=SHIFT(15),},
/*  12 */	{[T_EOF]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_ARROW]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_EQUAL]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_COMMA]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_SEMI]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_RBRACE]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_LBRACE]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_IGCASE]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_KEYWORD]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_TOKEN]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_SKIP]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_LOCAL_TOK]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),[T_REGEX]=REDUCE(P_N_FGFL_N_section_N_FGFL_PRIME),},
/*  13 */	{[T_KEYWORD]=SHIFT(8),[T_TOKEN]=SHIFT(7),[T_SKIP]=SHIFT(6),},
/*  14 */	{[T_EOF]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_ARROW]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_EQUAL]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_COMMA]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_SEMI]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_RBRACE]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_LBRACE]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_IGCASE]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_KEYWORD]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_TOKEN]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_SKIP]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_LOCAL_TOK]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),[T_REGEX]=REDUCE(P_N_FGFL_PRIME_N_section_N_FGFL_PRIME),},
/*  15 */	{[T_EOF]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_ARROW]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_EQUAL]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_COMMA]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_SEMI]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_RBRACE]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_LBRACE]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_IGCASE]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_KEYWORD]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_TOKEN]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_SKIP]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_LOCAL_TOK]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_GLOBAL_TOK]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),[T_REGEX]=REDUCE(P_N_atom_FGFL_T_GLOBAL_TOK),},
/*  16 */	{[T_EOF]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_ARROW]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_EQUAL]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_COMMA]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_SEMI]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_RBRACE]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_LBRACE]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_IGCASE]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_KEYWORD]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_TOKEN]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_SKIP]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_LOCAL_TOK]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_GLOBAL_TOK]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),[T_REGEX]=REDUCE(P_N_atom_FGFL_T_LOCAL_TOK),},
/*  17 */	{[T_ARROW]=SHIFT(29),[T_EQUAL]=SHIFT(28),},
/*  18 */	{[T_RBRACE]=SHIFT(26),},
/*  19 */	{[T_RBRACE]=SHIFT(25),},
/*  20 */	{[T_RBRACE]=SHIFT(24),},
/*  21 */	{[T_COMMA]=SHIFT(23),},
/*  22 */	{[T_EOF]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_ARROW]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_EQUAL]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_COMMA]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_SEMI]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_RBRACE]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_LBRACE]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_IGCASE]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_KEYWORD]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_TOKEN]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_SKIP]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_LOCAL_TOK]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_GLOBAL_TOK]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),[T_REGEX]=REDUCE(P_N_keyword_list_N_atom_FGFL_N_keyword_list_PRIME),},
/*  23 */	{[T_LOCAL_TOK]=SHIFT(16),[T_GLOBAL_TOK]=SHIFT(15),},
/*  24 */	{[T_SEMI]=SHIFT(35),},
/*  25 */	{[T_SEMI]=SHIFT(34),},
/*  26 */	{[T_SEMI]=SHIFT(33),},
/*  27 */	{[T_SEMI]=SHIFT(32),},
/*  28 */	{[T_REGEX]=SHIFT(31),},
/*  29 */	{[T_IGCASE]=SHIFT(30),},
/*  30 */	{[T_EOF]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_ARROW]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_EQUAL]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_COMMA]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_SEMI]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_RBRACE]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_LBRACE]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_IGCASE]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_KEYWORD]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_TOKEN]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_SKIP]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_LOCAL_TOK]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_GLOBAL_TOK]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),[T_REGEX]=REDUCE(P_N_regex_assign_T_ARROW_T_IGCASE),},
/*  31 */	{[T_EOF]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_ARROW]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_EQUAL]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_COMMA]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_SEMI]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_RBRACE]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_LBRACE]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_IGCASE]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_KEYWORD]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_TOKEN]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_SKIP]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_LOCAL_TOK]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_GLOBAL_TOK]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),[T_REGEX]=REDUCE(P_N_regex_assign_T_EQUAL_T_REGEX),},
/*  32 */	{[T_LOCAL_TOK]=SHIFT(16),[T_GLOBAL_TOK]=SHIFT(15),},
/*  33 */	{[T_EOF]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_IGCASE]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_KEYWORD]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_TOKEN]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_SKIP]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_LOCAL_TOK]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_REGEX]=REDUCE(P_N_skip_section_T_SKIP_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),},
/*  34 */	{[T_EOF]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_IGCASE]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_KEYWORD]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_TOKEN]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_SKIP]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_LOCAL_TOK]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),[T_REGEX]=REDUCE(P_N_token_section_T_TOKEN_T_LBRACE_N_regex_list_T_RBRACE_T_SEMI),},
/*  35 */	{[T_EOF]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_ARROW]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_EQUAL]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_COMMA]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_SEMI]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_RBRACE]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_LBRACE]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_IGCASE]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_KEYWORD]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_TOKEN]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_SKIP]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_LOCAL_TOK]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_GLOBAL_TOK]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),[T_REGEX]=REDUCE(P_N_keyword_section_T_KEYWORD_T_LBRACE_N_keyword_list_T_RBRACE_T_SEMI),},
/*  36 */	{[T_EOF]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_ARROW]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_EQUAL]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_COMMA]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_SEMI]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_RBRACE]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_LBRACE]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_IGCASE]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_KEYWORD]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_TOKEN]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_SKIP]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_LOCAL_TOK]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_GLOBAL_TOK]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),[T_REGEX]=REDUCE(P_N_keyword_list_PRIME_T_COMMA_N_keyword_list),},
/*  37 */	{[T_EOF]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_ARROW]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_EQUAL]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_COMMA]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_SEMI]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_RBRACE]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_LBRACE]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_IGCASE]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_KEYWORD]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_TOKEN]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_SKIP]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_LOCAL_TOK]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_GLOBAL_TOK]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),[T_REGEX]=REDUCE(P_N_regex_list_N_atom_FGFL_N_regex_assign_T_SEMI_N_regex_list),},
};

static int
lr_goto_table[38][TOTAL_TOKEN] = {
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
/*  10 */	{[N_atom_FGFL]=GOTO(17),[N_regex_list]=GOTO(19),},
/*  11 */	{[N_regex_list]=GOTO(18),[N_atom_FGFL]=GOTO(17),},
/*  12 */	{},
/*  13 */	{[N_FGFL_PRIME]=GOTO(14),[N_section]=GOTO(13),[N_keyword_section]=GOTO(5),[N_token_section]=GOTO(4),[N_skip_section]=GOTO(3),},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{},
/*  17 */	{[N_regex_assign]=GOTO(27),},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{},
/*  21 */	{[N_keyword_list_PRIME]=GOTO(22),},
/*  22 */	{},
/*  23 */	{[N_keyword_list]=GOTO(36),[N_atom_FGFL]=GOTO(21),},
/*  24 */	{},
/*  25 */	{},
/*  26 */	{},
/*  27 */	{},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{},
/*  31 */	{},
/*  32 */	{[N_regex_list]=GOTO(37),[N_atom_FGFL]=GOTO(17),},
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

#endif /* _FGFL_GRM_H */
