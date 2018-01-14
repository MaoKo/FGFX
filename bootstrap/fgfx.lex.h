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
	T_REG_UNION,
	T_REG_STAR,
	T_REG_PLUS,
	T_REG_QUES,
	T_REG_LPAREN,
	T_REG_RPAREN,
	T_REG_DOT,
	T_REG_LOOK,
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
	T_CCE_BLANK,
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
	T_N_CCE_BLANK,
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

#define TOTAL_TOKEN	91

#ifndef _ONLY_STATE_TOKEN_

static int8_t
fgfx_begin_table[TOTAL_TOKEN][TOTAL_STATE] = {
	[T_BEG_REGEX][S_GLOBAL] = S_IN_REGEX, 
	[T_END_REGEX][S_IN_REGEX] = S_GLOBAL, 
	[T_REG_QUOTE][S_IN_REGEX] = S_STRING, [T_REG_QUOTE][S_STRING] = S_IN_REGEX, 
	[T_REG_LBRACE][S_IN_REGEX] = S_FINITE_SEQ, 
	[T_REG_RBRACE][S_FINITE_SEQ] = S_IN_REGEX, 
	[T_REG_LBRACK][S_IN_REGEX] = S_BEG_CCL, 
	[T_REG_CARET][S_BEG_CCL] = S_BODY_CCL, 
	[T_REG_RBRACK][S_BEG_CCL] = S_IN_REGEX, [T_REG_RBRACK][S_BODY_CCL] = S_IN_REGEX, 
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
fgfx_IN_REGEX_state_table[24][256] = {
/*   0 */	{},
/*   1 */	{[125 ... 255]=2, [124]=16, [123]=15, [93 ... 122]=2, [92]=14, [91]=13, [65 ... 90]=2, [64]=12, [63]=11, [48 ... 62]=2, [47]=10, [46]=9, [44 ... 45]=2, [43]=8, [42]=7, [41]=6, [40]=5, [35 ... 39]=2, [34]=4, [33]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{[47]=10, [32]=17, [9]=17},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{},
/*  11 */	{},
/*  12 */	{},
/*  13 */	{},
/*  14 */	{[0 ... 255]=2},
/*  15 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20, [45]=19, [43]=18},
/*  16 */	{},
/*  17 */	{[47]=10, [32]=17, [9]=17},
/*  18 */	{[125]=21},
/*  19 */	{[125]=22},
/*  20 */	{[125]=23, [97 ... 122]=20, [95]=20, [65 ... 90]=20, [48 ... 57]=20},
/*  21 */	{},
/*  22 */	{},
/*  23 */	{},
};

static uint8_t
fgfx_IN_REGEX_final_table[19][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_QUOTE },
	{ 5, 	T_REG_LPAREN },
	{ 6, 	T_REG_RPAREN },
	{ 7, 	T_REG_STAR },
	{ 8, 	T_REG_PLUS },
	{ 9, 	T_REG_DOT },
	{ 10, 	T_END_REGEX },
	{ 11, 	T_REG_QUES },
	{ 12, 	T_REG_LOOK },
	{ 13, 	T_REG_LBRACK },
	{ 14, 	T_REG_CHAR },
	{ 15, 	T_REG_LBRACE },
	{ 16, 	T_REG_UNION },
	{ 21, 	T_REG_UNION_CLASS },
	{ 22, 	T_REG_DIFF_CLASS },
	{ 23, 	T_REG_BOUND_NAME },
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
fgfx_BEG_CCL_state_table[15][256] = {
/*   0 */	{},
/*   1 */	{[95 ... 255]=2, [94]=6, [93]=5, [92]=4, [91]=3, [11 ... 90]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[58]=7},
/*   4 */	{[0 ... 255]=2},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{[97 ... 122]=8, [95]=8, [94]=9, [65 ... 90]=8},
/*   8 */	{[97 ... 122]=8, [95]=8, [65 ... 90]=8, [58]=10},
/*   9 */	{[97 ... 122]=11, [95]=11, [65 ... 90]=11},
/*  10 */	{[93]=12},
/*  11 */	{[58]=13},
/*  12 */	{},
/*  13 */	{[93]=14},
/*  14 */	{},
};

static uint8_t
fgfx_BEG_CCL_final_table[8][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_RBRACK },
	{ 6, 	T_REG_CARET },
	{ 12, 	T_CCE },
	{ 14, 	T_N_CCE },
	{ 0 },
};

static uint8_t
fgfx_BODY_CCL_state_table[16][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=6, [92]=5, [91]=4, [46 ... 90]=2, [45]=3, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[94 ... 255]=7, [11 ... 92]=7, [0 ... 9]=7},
/*   4 */	{[58]=8},
/*   5 */	{[0 ... 255]=2},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{[97 ... 122]=9, [95]=9, [94]=10, [65 ... 90]=9},
/*   9 */	{[97 ... 122]=9, [95]=9, [65 ... 90]=9, [58]=11},
/*  10 */	{[97 ... 122]=12, [95]=12, [65 ... 90]=12},
/*  11 */	{[93]=13},
/*  12 */	{[58]=14},
/*  13 */	{},
/*  14 */	{[93]=15},
/*  15 */	{},
};

static uint8_t
fgfx_BODY_CCL_middle_table[16] = {
	[3]=true,
};

static uint8_t
fgfx_BODY_CCL_final_table[9][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REG_RBRACK },
	{ 7, 	T_REG_HYPHEN },
	{ 13, 	T_CCE },
	{ 15, 	T_N_CCE },
	{ 0 },
};

static int8_t
fgfx_look_table[2] = {
	T_REG_HYPHEN,
	-1,
};

static int8_t
fgfx_skip_table[3] = {
	T_SPACE,
	T_COMMENT,
	-1,
};

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _FGFX_LEX_H_ */
