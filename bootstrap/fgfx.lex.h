#ifndef _FGFX_LEX_H_
#define _FGFX_LEX_H_

#include <stdint.h>
#include <stdbool.h>

enum {
	DUMMY_STATE,
	S_GLOBAL,
	S_BEG_REGEX,
	S_BODY_REGEX,
	S_BEG_CCL,
	S_BODY_CCL,
	S_STRING,
	S_FINITE_SEQ,
};

#define TOTAL_STATE	8
#define INIT_STATE	S_GLOBAL

enum {
	T_EQUAL,
	T_STAR,
	T_OPEN_REGEX,
	T_CLOSE_REGEX,
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
	T_REG_RBRACE,
	T_DIGIT,
	T_REG_COMMA,
	T_REG_DIFF_CLASS,
	T_REG_UNION_CLASS,
	T_REG_LBRACK,
	T_REG_RBRACK,
	T_REG_CARET,
	T_REG_DOLLAR,
	T_REG_HYPHEN,
	T_CCE,
	T_N_CCE,
	T_OCT_NUM,
	T_HEX_NUM,
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
	T_MULTI_LINE,
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
	T_CC_FIRST,
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
	T_CC_MIDDLE,
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
	T_CC_LAST,
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

#define TOTAL_TOKEN	98

#ifndef _ONLY_STATE_TOKEN_

static int8_t
fgfx_begin_table[TOTAL_TOKEN][TOTAL_STATE] = {
	[T_OPEN_REGEX][S_GLOBAL] = S_BEG_REGEX, 
	[T_CLOSE_REGEX][S_BEG_REGEX] = S_GLOBAL, [T_CLOSE_REGEX][S_BODY_REGEX] = S_GLOBAL, 
	[T_REG_UNION][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_STAR][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_PLUS][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_QUES][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_LPAREN][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_RPAREN][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_DOT][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_LOOK][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_QUOTE][S_BEG_REGEX] = S_STRING, [T_REG_QUOTE][S_BODY_REGEX] = S_STRING, [T_REG_QUOTE][S_STRING] = S_BODY_REGEX, 
	[T_REG_BOUND_NAME][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_LBRACE][S_BEG_REGEX] = S_FINITE_SEQ, [T_REG_LBRACE][S_BODY_REGEX] = S_FINITE_SEQ, 
	[T_REG_RBRACE][S_FINITE_SEQ] = S_BODY_REGEX, 
	[T_REG_LBRACK][S_BEG_REGEX] = S_BEG_CCL, [T_REG_LBRACK][S_BODY_REGEX] = S_BEG_CCL, 
	[T_REG_RBRACK][S_BEG_CCL] = S_BODY_REGEX, [T_REG_RBRACK][S_BODY_CCL] = S_BODY_REGEX, 
	[T_REG_CARET][S_BEG_REGEX] = S_BODY_REGEX, [T_REG_CARET][S_BEG_CCL] = S_BODY_CCL, 
	[T_CCE][S_BEG_CCL] = S_BODY_CCL, 
	[T_N_CCE][S_BEG_CCL] = S_BODY_CCL, 
	[T_OCT_NUM][S_BEG_CCL] = S_BODY_CCL, [T_OCT_NUM][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_HEX_NUM][S_BEG_CCL] = S_BODY_CCL, [T_HEX_NUM][S_BEG_REGEX] = S_BODY_REGEX, 
	[T_REG_CHAR][S_BEG_CCL] = S_BODY_CCL, [T_REG_CHAR][S_BEG_REGEX] = S_BODY_REGEX, 
};

#define START_STATE	1
#define DEAD_STATE	0

static uint8_t
fgfx_GLOBAL_state_table[36][256] = {
/*   0 */	{},
/*   1 */	{[125]=15, [124]=14, [123]=13, [97 ... 122]=10, [95]=10, [93]=12, [91]=11, [65 ... 90]=10, [61]=9, [60]=25, [59]=8, [47]=7, [45]=24, [44]=6, [42]=5, [41]=4, [40]=3, [39]=23, [36]=22, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{[47]=18, [42]=29, [32]=28, [9]=28},
/*   8 */	{},
/*   9 */	{[62]=19},
/*  10 */	{[97 ... 122]=10, [95]=10, [65 ... 90]=10, [48 ... 57]=10},
/*  11 */	{},
/*  12 */	{},
/*  13 */	{},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{[97 ... 122]=16, [95]=16, [65 ... 90]=16, [48 ... 57]=16},
/*  17 */	{},
/*  18 */	{[11 ... 255]=18, [0 ... 9]=18},
/*  19 */	{},
/*  20 */	{},
/*  21 */	{},
/*  22 */	{[97 ... 122]=16, [95]=16, [65 ... 90]=16},
/*  23 */	{[93 ... 255]=26, [92]=27, [40 ... 91]=26, [35 ... 38]=26, [11 ... 33]=26, [0 ... 9]=26},
/*  24 */	{[62]=17},
/*  25 */	{[97 ... 122]=30, [95]=30, [65 ... 90]=30},
/*  26 */	{[93 ... 255]=26, [92]=27, [40 ... 91]=26, [39]=20, [35 ... 38]=26, [11 ... 33]=26, [0 ... 9]=26},
/*  27 */	{[92]=26, [39]=26, [34]=26},
/*  28 */	{[32]=28, [9]=28},
/*  29 */	{[43 ... 255]=29, [42]=31, [0 ... 41]=29},
/*  30 */	{[97 ... 122]=30, [95]=30, [65 ... 90]=30, [62]=21, [48 ... 57]=30, [39]=32},
/*  31 */	{[48 ... 255]=29, [47]=33, [43 ... 46]=29, [42]=31, [0 ... 41]=29},
/*  32 */	{[62]=21, [39]=34},
/*  33 */	{},
/*  34 */	{[62]=21, [39]=35},
/*  35 */	{[62]=21},
};

static uint8_t
fgfx_GLOBAL_final_table[23][2] = {
	{ 2, 	T_SPACE },
	{ 3, 	T_LPAREN },
	{ 4, 	T_RPAREN },
	{ 5, 	T_STAR },
	{ 6, 	T_COMMA },
	{ 7, 	T_OPEN_REGEX },
	{ 8, 	T_SEMI },
	{ 9, 	T_EQUAL },
	{ 10, 	T_TERMINAL },
	{ 11, 	T_LBRACK },
	{ 12, 	T_RBRACK },
	{ 13, 	T_LBRACE },
	{ 14, 	T_UNION },
	{ 15, 	T_RBRACE },
	{ 16, 	T_DIRECTIVE },
	{ 17, 	T_ARROW },
	{ 18, 	T_COMMENT },
	{ 19, 	T_BARROW },
	{ 20, 	T_LITERAL },
	{ 21, 	T_NON_TERMINAL },
	{ 28, 	T_OPEN_REGEX },
	{ 33, 	T_COMMENT },
	{ 0 },
};

static uint8_t
fgfx_BEG_REGEX_state_table[29][256] = {
/*   0 */	{},
/*   1 */	{[125 ... 255]=2, [124]=14, [123]=13, [95 ... 122]=2, [94]=12, [93]=2, [92]=21, [91]=11, [64 ... 90]=2, [63]=10, [48 ... 62]=2, [47]=9, [46]=8, [44 ... 45]=2, [43]=7, [42]=6, [41]=5, [40]=4, [35 ... 39]=2, [34]=3, [33]=2, [32]=20, [11 ... 31]=2, [9]=20, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[59]=15, [45]=23, [44]=15, [32]=22, [9 ... 10]=22},
/*  10 */	{},
/*  11 */	{},
/*  12 */	{},
/*  13 */	{[97 ... 122]=25, [95]=25, [65 ... 90]=25},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{[92]=26, [32]=16, [9]=16},
/*  17 */	{[48 ... 55]=27},
/*  18 */	{[97 ... 102]=28, [65 ... 70]=28, [48 ... 57]=28},
/*  19 */	{},
/*  20 */	{[47]=22, [32]=20, [9]=20},
/*  21 */	{[121 ... 255]=2, [120]=24, [89 ... 119]=2, [88]=24, [56 ... 87]=2, [48 ... 55]=17, [11 ... 47]=2, [10]=16, [0 ... 9]=2},
/*  22 */	{[59]=15, [45]=23, [44]=15, [32]=22, [9 ... 10]=22},
/*  23 */	{[62]=15},
/*  24 */	{[97 ... 102]=18, [65 ... 70]=18, [48 ... 57]=18},
/*  25 */	{[125]=19, [97 ... 122]=25, [95]=25, [65 ... 90]=25, [48 ... 57]=25},
/*  26 */	{[10]=16},
/*  27 */	{[48 ... 55]=28},
/*  28 */	{},
};

static uint8_t
fgfx_BEG_REGEX_middle_table[29] = {
	[9]=true,
	[22]=true,
};

static uint8_t
fgfx_BEG_REGEX_final_table[24][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_REG_LPAREN },
	{ 5, 	T_REG_RPAREN },
	{ 6, 	T_REG_STAR },
	{ 7, 	T_REG_PLUS },
	{ 8, 	T_REG_DOT },
	{ 9, 	T_REG_LOOK },
	{ 10, 	T_REG_QUES },
	{ 11, 	T_REG_LBRACK },
	{ 12, 	T_REG_CARET },
	{ 13, 	T_REG_LBRACE },
	{ 14, 	T_REG_UNION },
	{ 15, 	T_CLOSE_REGEX },
	{ 16, 	T_MULTI_LINE },
	{ 17, 	T_OCT_NUM },
	{ 18, 	T_HEX_NUM },
	{ 19, 	T_REG_BOUND_NAME },
	{ 20, 	T_REG_CHAR },
	{ 21, 	T_REG_CHAR },
	{ 24, 	T_REG_CHAR },
	{ 27, 	T_OCT_NUM },
	{ 28, 	T_OCT_NUM },
	{ 0 },
};

static uint8_t
fgfx_BODY_REGEX_state_table[34][256] = {
/*   0 */	{},
/*   1 */	{[125 ... 255]=2, [124]=13, [123]=12, [93 ... 122]=2, [92]=24, [91]=11, [64 ... 90]=2, [63]=10, [48 ... 62]=2, [47]=9, [46]=8, [44 ... 45]=2, [43]=7, [42]=6, [41]=5, [40]=4, [37 ... 39]=2, [36]=23, [35]=2, [34]=3, [33]=2, [32]=22, [11 ... 31]=2, [9]=22, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[59]=15, [45]=26, [44]=15, [32]=25, [9 ... 10]=25},
/*  10 */	{},
/*  11 */	{},
/*  12 */	{[97 ... 122]=30, [95]=30, [65 ... 90]=30, [45]=29, [43]=28},
/*  13 */	{},
/*  14 */	{},
/*  15 */	{},
/*  16 */	{[92]=31, [32]=16, [9]=16},
/*  17 */	{[48 ... 55]=32},
/*  18 */	{[97 ... 102]=33, [65 ... 70]=33, [48 ... 57]=33},
/*  19 */	{},
/*  20 */	{},
/*  21 */	{},
/*  22 */	{[47]=25, [32]=22, [9]=22},
/*  23 */	{[47]=14, [32]=23, [9]=23},
/*  24 */	{[121 ... 255]=2, [120]=27, [89 ... 119]=2, [88]=27, [56 ... 87]=2, [48 ... 55]=17, [11 ... 47]=2, [10]=16, [0 ... 9]=2},
/*  25 */	{[59]=15, [45]=26, [44]=15, [32]=25, [9 ... 10]=25},
/*  26 */	{[62]=15},
/*  27 */	{[97 ... 102]=18, [65 ... 70]=18, [48 ... 57]=18},
/*  28 */	{[125]=19},
/*  29 */	{[125]=20},
/*  30 */	{[125]=21, [97 ... 122]=30, [95]=30, [65 ... 90]=30, [48 ... 57]=30},
/*  31 */	{[10]=16},
/*  32 */	{[48 ... 55]=33},
/*  33 */	{},
};

static uint8_t
fgfx_BODY_REGEX_middle_table[34] = {
	[9]=true,
	[23]=true,
	[25]=true,
};

static uint8_t
fgfx_BODY_REGEX_final_table[27][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_REG_LPAREN },
	{ 5, 	T_REG_RPAREN },
	{ 6, 	T_REG_STAR },
	{ 7, 	T_REG_PLUS },
	{ 8, 	T_REG_DOT },
	{ 9, 	T_REG_LOOK },
	{ 10, 	T_REG_QUES },
	{ 11, 	T_REG_LBRACK },
	{ 12, 	T_REG_LBRACE },
	{ 13, 	T_REG_UNION },
	{ 14, 	T_REG_DOLLAR },
	{ 15, 	T_CLOSE_REGEX },
	{ 16, 	T_MULTI_LINE },
	{ 17, 	T_OCT_NUM },
	{ 18, 	T_HEX_NUM },
	{ 19, 	T_REG_UNION_CLASS },
	{ 20, 	T_REG_DIFF_CLASS },
	{ 21, 	T_REG_BOUND_NAME },
	{ 22, 	T_REG_CHAR },
	{ 23, 	T_REG_CHAR },
	{ 24, 	T_REG_CHAR },
	{ 27, 	T_REG_CHAR },
	{ 32, 	T_OCT_NUM },
	{ 33, 	T_OCT_NUM },
	{ 0 },
};

static uint8_t
fgfx_BEG_CCL_state_table[22][256] = {
/*   0 */	{},
/*   1 */	{[95 ... 255]=2, [94]=4, [93]=3, [92]=11, [91]=10, [11 ... 90]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{[92]=16, [32]=5, [9]=5},
/*   6 */	{[48 ... 55]=17},
/*   7 */	{[97 ... 102]=20, [65 ... 70]=20, [48 ... 57]=20},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{[58]=12},
/*  11 */	{[121 ... 255]=2, [120]=13, [89 ... 119]=2, [88]=13, [56 ... 87]=2, [48 ... 55]=6, [11 ... 47]=2, [10]=5, [0 ... 9]=2},
/*  12 */	{[97 ... 122]=14, [95]=14, [94]=15, [65 ... 90]=14},
/*  13 */	{[97 ... 102]=7, [65 ... 70]=7, [48 ... 57]=7},
/*  14 */	{[97 ... 122]=14, [95]=14, [65 ... 90]=14, [58]=18},
/*  15 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19},
/*  16 */	{[10]=5},
/*  17 */	{[48 ... 55]=20},
/*  18 */	{[93]=8},
/*  19 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19, [58]=21},
/*  20 */	{},
/*  21 */	{[93]=9},
};

static uint8_t
fgfx_BEG_CCL_final_table[14][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_RBRACK },
	{ 4, 	T_REG_CARET },
	{ 5, 	T_MULTI_LINE },
	{ 6, 	T_OCT_NUM },
	{ 7, 	T_HEX_NUM },
	{ 8, 	T_CCE },
	{ 9, 	T_N_CCE },
	{ 10, 	T_REG_CHAR },
	{ 11, 	T_REG_CHAR },
	{ 13, 	T_REG_CHAR },
	{ 17, 	T_OCT_NUM },
	{ 20, 	T_OCT_NUM },
	{ 0 },
};

static uint8_t
fgfx_BODY_CCL_state_table[23][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=3, [92]=12, [91]=11, [46 ... 90]=2, [45]=10, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{[92]=17, [32]=5, [9]=5},
/*   6 */	{[48 ... 55]=18},
/*   7 */	{[97 ... 102]=21, [65 ... 70]=21, [48 ... 57]=21},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{[94 ... 255]=4, [11 ... 92]=4, [0 ... 9]=4},
/*  11 */	{[58]=13},
/*  12 */	{[121 ... 255]=2, [120]=14, [89 ... 119]=2, [88]=14, [56 ... 87]=2, [48 ... 55]=6, [11 ... 47]=2, [10]=5, [0 ... 9]=2},
/*  13 */	{[97 ... 122]=15, [95]=15, [94]=16, [65 ... 90]=15},
/*  14 */	{[97 ... 102]=7, [65 ... 70]=7, [48 ... 57]=7},
/*  15 */	{[97 ... 122]=15, [95]=15, [65 ... 90]=15, [58]=19},
/*  16 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20},
/*  17 */	{[10]=5},
/*  18 */	{[48 ... 55]=21},
/*  19 */	{[93]=8},
/*  20 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20, [58]=22},
/*  21 */	{},
/*  22 */	{[93]=9},
};

static uint8_t
fgfx_BODY_CCL_middle_table[23] = {
	[10]=true,
};

static uint8_t
fgfx_BODY_CCL_final_table[15][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_RBRACK },
	{ 4, 	T_REG_HYPHEN },
	{ 5, 	T_MULTI_LINE },
	{ 6, 	T_OCT_NUM },
	{ 7, 	T_HEX_NUM },
	{ 8, 	T_CCE },
	{ 9, 	T_N_CCE },
	{ 10, 	T_REG_CHAR },
	{ 11, 	T_REG_CHAR },
	{ 12, 	T_REG_CHAR },
	{ 14, 	T_REG_CHAR },
	{ 18, 	T_OCT_NUM },
	{ 21, 	T_OCT_NUM },
	{ 0 },
};

static uint8_t
fgfx_STRING_state_table[12][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=7, [35 ... 91]=2, [34]=3, [11 ... 33]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[92]=9, [32]=4, [9]=4},
/*   5 */	{[48 ... 55]=10},
/*   6 */	{[97 ... 102]=11, [65 ... 70]=11, [48 ... 57]=11},
/*   7 */	{[121 ... 255]=2, [120]=8, [89 ... 119]=2, [88]=8, [56 ... 87]=2, [48 ... 55]=5, [11 ... 47]=2, [10]=4, [0 ... 9]=2},
/*   8 */	{[97 ... 102]=6, [65 ... 70]=6, [48 ... 57]=6},
/*   9 */	{[10]=4},
/*  10 */	{[48 ... 55]=11},
/*  11 */	{},
};

static uint8_t
fgfx_STRING_final_table[10][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_MULTI_LINE },
	{ 5, 	T_OCT_NUM },
	{ 6, 	T_HEX_NUM },
	{ 7, 	T_REG_CHAR },
	{ 8, 	T_REG_CHAR },
	{ 10, 	T_OCT_NUM },
	{ 11, 	T_OCT_NUM },
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

static int8_t
fgfx_look_table[4] = {
	T_CLOSE_REGEX,
	T_REG_DOLLAR,
	T_REG_HYPHEN,
	-1,
};

static int8_t
fgfx_skip_table[4] = {
	T_SPACE,
	T_MULTI_LINE,
	T_COMMENT,
	-1,
};

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _FGFX_LEX_H_ */
