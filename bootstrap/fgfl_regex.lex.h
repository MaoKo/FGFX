#ifndef _FGFL_REGEX_LEX_H_
#define _FGFL_REGEX_LEX_H_

#include <stdint.h>

enum {
	S_GLOBAL,
	S_STRING,
	S_FINITE_SEQ,
	S_BEG_CCL,
	S_BODY_CCL,
};

#define TOTAL_STATE	5
#define INIT_STATE	S_GLOBAL

enum {
	T_UNION,
	T_STAR,
	T_PLUS,
	T_QUES,
	T_LPAREN,
	T_RPAREN,
	T_DOT,
	T_QUOTE,
	T_BOUND_NAME,
	T_LBRACE,
	T_DIGIT,
	T_COMMA,
	T_RBRACE,
	T_DIFF_CLASS,
	T_UNION_CLASS,
	T_LBRACK,
	T_CARET,
	T_HYPHEN,
	T_RBRACK,
	T_CHAR,
	T_ERROR,
	T_EOF,
};

#define TOTAL_TOKEN	22

#ifndef _ONLY_STATE_TOKEN_

static int8_t
fgfl_regex_begin_table[TOTAL_TOKEN][TOTAL_STATE] = {
	[T_QUOTE][S_GLOBAL] = S_STRING, [T_QUOTE][S_STRING] = S_GLOBAL, 
	[T_LBRACE][S_GLOBAL] = S_FINITE_SEQ, 
	[T_RBRACE][S_FINITE_SEQ] = S_GLOBAL, 
	[T_LBRACK][S_GLOBAL] = S_BEG_CCL, 
	[T_CARET][S_BEG_CCL] = S_BODY_CCL, 
	[T_RBRACK][S_BODY_CCL] = S_GLOBAL, 
	[T_CHAR][S_BEG_CCL] = S_BODY_CCL, 
};

#define START_STATE	1
#define DEAD_STATE	0

static uint8_t
fgfl_regex_GLOBAL_state_table[19][256] = {
/*   0 */	{},
/*   1 */	{[124]=11, [123]=10, [91]=9, [63]=8, [46]=7, [43]=6, [42]=5, [41]=4, [40]=3, [34]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{[97 ... 122]=14, [95]=14, [65 ... 90]=14, [45]=13, [43]=12},
/*  11 */	{},
/*  12 */	{[125]=15},
/*  13 */	{[125]=16},
/*  14 */	{[125]=18, [97 ... 122]=14, [95]=14, [65 ... 90]=14, [48 ... 57]=17},
/*  15 */	{},
/*  16 */	{},
/*  17 */	{[125]=18, [97 ... 122]=17, [95]=17, [65 ... 90]=17, [48 ... 57]=17},
/*  18 */	{},
};

static uint8_t
fgfl_regex_GLOBAL_final_table[14][2] = {
	{ 2, 	T_QUOTE },
	{ 3, 	T_LPAREN },
	{ 4, 	T_RPAREN },
	{ 5, 	T_STAR },
	{ 6, 	T_PLUS },
	{ 7, 	T_DOT },
	{ 8, 	T_QUES },
	{ 9, 	T_LBRACK },
	{ 10, 	T_LBRACE },
	{ 11, 	T_UNION },
	{ 15, 	T_UNION_CLASS },
	{ 16, 	T_DIFF_CLASS },
	{ 18, 	T_BOUND_NAME },
	{ 0 },
};

static uint8_t
fgfl_regex_STRING_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=4, [35 ... 91]=2, [34]=3, [11 ... 33]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[0 ... 255]=2},
};

static uint8_t
fgfl_regex_STRING_final_table[4][2] = {
	{ 2, 	T_CHAR },
	{ 3, 	T_QUOTE },
	{ 4, 	T_CHAR },
	{ 0 },
};

static uint8_t
fgfl_regex_FINITE_SEQ_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[125]=4, [48 ... 57]=3, [44]=2},
/*   2 */	{},
/*   3 */	{[48 ... 57]=3},
/*   4 */	{},
};

static uint8_t
fgfl_regex_FINITE_SEQ_final_table[4][2] = {
	{ 2, 	T_COMMA },
	{ 3, 	T_DIGIT },
	{ 4, 	T_RBRACE },
	{ 0 },
};

static uint8_t
fgfl_regex_BEG_CCL_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[95 ... 255]=2, [94]=4, [93]=2, [92]=3, [11 ... 91]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[0 ... 255]=2},
/*   4 */	{},
};

static uint8_t
fgfl_regex_BEG_CCL_final_table[4][2] = {
	{ 2, 	T_CHAR },
	{ 3, 	T_CHAR },
	{ 4, 	T_CARET },
	{ 0 },
};

static uint8_t
fgfl_regex_BODY_CCL_state_table[6][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=5, [92]=4, [46 ... 91]=2, [45]=3, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[0 ... 255]=2},
/*   5 */	{},
};

static uint8_t
fgfl_regex_BODY_CCL_final_table[5][2] = {
	{ 2, 	T_CHAR },
	{ 3, 	T_HYPHEN },
	{ 4, 	T_CHAR },
	{ 5, 	T_RBRACK },
	{ 0 },
};

#define SKIP_TABLE_NOT_DEFINE	

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _FGFL_REGEX_LEX_H_ */
