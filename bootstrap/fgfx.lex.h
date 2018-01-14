#ifndef _FGFX_LEX_H_
#define _FGFX_LEX_H_

#include <stdint.h>
#include <stdbool.h>

enum {
	DUMMY_STATE,
	S_GLOBAL,
	S_IN_REGEX,
	S_STRING,
	S_FINITE_SEQ,
	S_BEG_CCL,
	S_BODY_CCL,
};

#define TOTAL_STATE	7
#define INIT_STATE	S_GLOBAL

enum {
	T_EQUAL,
	T_STAR,
	T_BEG_REGEX,
	T_END_REGEX,
	T_REGEX,
	T_REG_UNION,
	T_REG_STAR,
	T_REG_PLUS,
	T_REG_QUES,
	T_REG_LPAREN,
	T_REG_RPAREN,
	T_REG_DOT,
	T_REG_QUOTE,
	T_REG_BOUND_NAME,
	T_REG_LBRACE,
	T_DIGIT,
	T_REG_COMMA,
	T_REG_RBRACE,
	T_REG_DIFF_CLASS,
	T_REG_UNION_CLASS,
	T_REG_LBRACK,
	T_REG_CARET,
	T_REG_HYPHEN,
	T_REG_RBRACK,
	T_CCE,
	T_N_CCE,
	T_REG_CHAR,
	T_NON_TERMINAL,
	T_LITERAL,
	T_UNION,
	T_LBRACK,
	T_RBRACK,
	T_DIRECTIVE,
	T_TERMINAL,
	T_ARROW,
	T_BARROW,
	T_SEMI,
	T_COMMA,
	T_LBRACE,
	T_RBRACE,
	T_LPAREN,
	T_RPAREN,
	T_SPACE,
	T_COMMENT,
	T_SKIP,
	T_TOKEN,
	T_KEYWORD,
	T_IGCASE,
	T_STATE,
	T_BEGIN,
	T_FRAG,
	T_INITIAL,
	T_ALL,
	T_NONE,
	T_REJECT,
	T_CCE_ALNUM,
	T_CCE_ALPHA,
	T_CCE_CNTRL,
	T_CCE_DIGIT,
	T_CCE_GRAPH,
	T_CCE_LOWER,
	T_CCE_PRINT,
	T_CCE_PUNCT,
	T_CCE_SPACE,
	T_CCE_UPPER,
	T_CCE_XDIGIT,
	T_N_CCE_ALNUM,
	T_N_CCE_ALPHA,
	T_N_CCE_CNTRL,
	T_N_CCE_DIGIT,
	T_N_CCE_GRAPH,
	T_N_CCE_LOWER,
	T_N_CCE_PRINT,
	T_N_CCE_PUNCT,
	T_N_CCE_SPACE,
	T_N_CCE_UPPER,
	T_N_CCE_XDIGIT,
	T_EXTERN,
	T_PRODUCTION,
	T_ALIAS,
	T_PRECEDENCE,
	T_MIMIC,
	T_EMPTY,
	T_START,
	T_LEFT,
	T_RIGHT,
	T_NONASSOC,
	T_ERROR,
	T_EOF,
};

#define TOTAL_TOKEN	89

#ifndef _ONLY_STATE_TOKEN_

static int8_t
fgfx_begin_table[TOTAL_TOKEN][TOTAL_STATE] = {
	[T_BEG_REGEX][S_GLOBAL] = S_IN_REGEX, 
	[T_END_REGEX][S_IN_REGEX] = S_GLOBAL, 
	[T_REG_QUOTE][S_IN_REGEX] = S_STRING, [T_REG_QUOTE][S_STRING] = S_IN_REGEX, 
	[T_REG_LBRACE][S_IN_REGEX] = S_FINITE_SEQ, 
	[T_REG_RBRACE][S_FINITE_SEQ] = S_IN_REGEX, 
	[T_REG_LBRACK][S_IN_REGEX] = S_BEG_CCL, 
	[T_REG_CARET][S_IN_REGEX] = S_BODY_CCL, 
	[T_REG_RBRACK][S_BODY_CCL] = S_IN_REGEX, 
	[T_CCE][S_BEG_CCL] = S_BODY_CCL, 
	[T_N_CCE][S_BEG_CCL] = S_BODY_CCL, 
	[T_REG_CHAR][S_BEG_CCL] = S_BODY_CCL, 
};

#define START_STATE	1
#define DEAD_STATE	0

static uint8_t
fgfx_GLOBAL_state_table[36][256] = {
/*   0 */	{},
/*   1 */	{[125]=19, [124]=18, [123]=17, [97 ... 122]=14, [95]=14, [93]=16, [91]=15, [65 ... 90]=14, [61]=13, [60]=12, [59]=11, [47]=10, [45]=9, [44]=8, [42]=7, [41]=6, [40]=5, [39]=4, [36]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20},
/*   4 */	{[93 ... 255]=21, [92]=22, [40 ... 91]=21, [35 ... 38]=21, [11 ... 33]=21, [0 ... 9]=21},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[62]=23},
/*  10 */	{[47]=26, [42]=25, [32]=24, [9]=24},
/*  11 */	{},
/*  12 */	{[97 ... 122]=27, [95]=27, [65 ... 90]=27},
/*  13 */	{[62]=28},
/*  14 */	{[97 ... 122]=14, [95]=14, [65 ... 90]=14, [48 ... 57]=14},
/*  15 */	{},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20, [48 ... 57]=20},
/*  21 */	{[93 ... 255]=21, [92]=22, [40 ... 91]=21, [39]=29, [35 ... 38]=21, [11 ... 33]=21, [0 ... 9]=21},
/*  22 */	{[92]=21, [39]=21, [34]=21},
/*  23 */	{},
/*  24 */	{[32]=24, [9]=24},
/*  25 */	{[43 ... 255]=25, [42]=30, [0 ... 41]=25},
/*  26 */	{[11 ... 255]=26, [0 ... 9]=26},
/*  27 */	{[97 ... 122]=27, [95]=27, [65 ... 90]=27, [62]=32, [48 ... 57]=27, [39]=31},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{[48 ... 255]=25, [47]=33, [43 ... 46]=25, [42]=30, [0 ... 41]=25},
/*  31 */	{[62]=32, [39]=34},
/*  32 */	{},
/*  33 */	{},
/*  34 */	{[62]=32, [39]=35},
/*  35 */	{[62]=32},
};

static uint8_t
fgfx_GLOBAL_final_table[23][2] = {
	{ 2, 	T_SPACE },
	{ 5, 	T_LPAREN },
	{ 6, 	T_RPAREN },
	{ 7, 	T_STAR },
	{ 8, 	T_COMMA },
	{ 10, 	T_BEG_REGEX },
	{ 11, 	T_SEMI },
	{ 13, 	T_EQUAL },
	{ 14, 	T_TERMINAL },
	{ 15, 	T_LBRACK },
	{ 16, 	T_RBRACK },
	{ 17, 	T_LBRACE },
	{ 18, 	T_UNION },
	{ 19, 	T_RBRACE },
	{ 20, 	T_DIRECTIVE },
	{ 23, 	T_ARROW },
	{ 24, 	T_BEG_REGEX },
	{ 26, 	T_COMMENT },
	{ 28, 	T_BARROW },
	{ 29, 	T_LITERAL },
	{ 32, 	T_NON_TERMINAL },
	{ 33, 	T_COMMENT },
	{ 0 },
};

static uint8_t
fgfx_IN_REGEX_state_table[9][256] = {
/*   0 */	{},
/*   1 */	{[124 ... 255]=2, [123]=6, [93 ... 122]=2, [92]=5, [48 ... 91]=2, [47]=4, [33 ... 46]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   2 */	{[93 ... 255]=2, [92]=7, [48 ... 91]=2, [11 ... 46]=2, [0 ... 9]=2},
/*   3 */	{[93 ... 255]=2, [92]=7, [48 ... 91]=2, [47]=4, [33 ... 46]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   4 */	{},
/*   5 */	{[0 ... 255]=2},
/*   6 */	{[123 ... 255]=2, [97 ... 122]=8, [96]=2, [95]=8, [93 ... 94]=2, [92]=7, [91]=2, [65 ... 90]=8, [48 ... 64]=2, [11 ... 46]=2, [0 ... 9]=2},
/*   7 */	{[0 ... 255]=2},
/*   8 */	{[123 ... 255]=2, [97 ... 122]=8, [96]=2, [95]=8, [93 ... 94]=2, [92]=7, [91]=2, [65 ... 90]=8, [58 ... 64]=2, [48 ... 57]=8, [11 ... 46]=2, [0 ... 9]=2},
};

static uint8_t
fgfx_IN_REGEX_final_table[8][2] = {
	{ 1, 	T_REGEX },
	{ 2, 	T_REGEX },
	{ 3, 	T_REGEX },
	{ 4, 	T_END_REGEX },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REGEX },
	{ 8, 	T_REGEX },
	{ 0 },
};

static uint8_t
fgfx_STRING_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=4, [35 ... 91]=2, [34]=3, [11 ... 33]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[0 ... 255]=2},
};

static uint8_t
fgfx_STRING_final_table[4][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_REG_CHAR },
	{ 0 },
};

static uint8_t
fgfx_FINITE_SEQ_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[125]=4, [48 ... 57]=3, [44]=2},
/*   2 */	{},
/*   3 */	{[48 ... 57]=3},
/*   4 */	{},
};

static uint8_t
fgfx_FINITE_SEQ_final_table[4][2] = {
	{ 2, 	T_REG_COMMA },
	{ 3, 	T_DIGIT },
	{ 4, 	T_REG_RBRACE },
	{ 0 },
};

static uint8_t
fgfx_BEG_CCL_state_table[27][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=4, [91]=3, [11 ... 90]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[58]=5},
/*   4 */	{[0 ... 255]=2},
/*   5 */	{[123]=7, [94]=6},
/*   6 */	{[123]=8},
/*   7 */	{[76]=9},
/*   8 */	{[76]=10},
/*   9 */	{[69]=11},
/*  10 */	{[69]=12},
/*  11 */	{[84]=13},
/*  12 */	{[84]=14},
/*  13 */	{[84]=15},
/*  14 */	{[84]=16},
/*  15 */	{[69]=17},
/*  16 */	{[69]=18},
/*  17 */	{[82]=19},
/*  18 */	{[82]=20},
/*  19 */	{[125]=21},
/*  20 */	{[125]=22},
/*  21 */	{[58]=23},
/*  22 */	{[58]=24},
/*  23 */	{[93]=25},
/*  24 */	{[93]=26},
/*  25 */	{},
/*  26 */	{},
};

static uint8_t
fgfx_BEG_CCL_final_table[6][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 25, 	T_CCE },
	{ 26, 	T_N_CCE },
	{ 0 },
};

static uint8_t
fgfx_BODY_CCL_state_table[29][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=6, [92]=5, [91]=4, [46 ... 90]=2, [45]=3, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[58]=7},
/*   5 */	{[0 ... 255]=2},
/*   6 */	{},
/*   7 */	{[123]=9, [94]=8},
/*   8 */	{[123]=10},
/*   9 */	{[76]=11},
/*  10 */	{[76]=12},
/*  11 */	{[69]=13},
/*  12 */	{[69]=14},
/*  13 */	{[84]=15},
/*  14 */	{[84]=16},
/*  15 */	{[84]=17},
/*  16 */	{[84]=18},
/*  17 */	{[69]=19},
/*  18 */	{[69]=20},
/*  19 */	{[82]=21},
/*  20 */	{[82]=22},
/*  21 */	{[125]=23},
/*  22 */	{[125]=24},
/*  23 */	{[58]=25},
/*  24 */	{[58]=26},
/*  25 */	{[93]=27},
/*  26 */	{[93]=28},
/*  27 */	{},
/*  28 */	{},
};

static uint8_t
fgfx_BODY_CCL_final_table[8][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_HYPHEN },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REG_RBRACK },
	{ 27, 	T_CCE },
	{ 28, 	T_N_CCE },
	{ 0 },
};

#define LOOK_TABLE_NOT_DEFINE	

static int8_t
fgfx_skip_table[3] = {
	T_SPACE,
	T_COMMENT,
	-1,
};

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _FGFX_LEX_H_ */
