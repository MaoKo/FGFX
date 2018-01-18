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

#define TOTAL_TOKEN	96

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
	{ 10, 	T_OPEN_REGEX },
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
	{ 24, 	T_OPEN_REGEX },
	{ 26, 	T_COMMENT },
	{ 28, 	T_BARROW },
	{ 29, 	T_LITERAL },
	{ 32, 	T_NON_TERMINAL },
	{ 33, 	T_COMMENT },
	{ 0 },
};

static uint8_t
fgfx_BEG_REGEX_state_table[29][256] = {
/*   0 */	{},
/*   1 */	{[125 ... 255]=2, [124]=16, [123]=15, [95 ... 122]=2, [94]=14, [93]=2, [92]=13, [91]=12, [64 ... 90]=2, [63]=11, [48 ... 62]=2, [47]=10, [46]=9, [44 ... 45]=2, [43]=8, [42]=7, [41]=6, [40]=5, [35 ... 39]=2, [34]=4, [33]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{[47]=18, [32]=17, [9]=17},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{[59]=19, [45]=20, [44]=19, [32]=18, [9 ... 10]=18},
/*  11 */	{},
/*  12 */	{},
/*  13 */	{[121 ... 255]=2, [120]=22, [89 ... 119]=2, [88]=22, [56 ... 87]=2, [48 ... 55]=21, [0 ... 47]=2},
/*  14 */	{},
/*  15 */	{[97 ... 122]=23, [95]=23, [65 ... 90]=23},
/*  16 */	{},
/*  17 */	{[47]=18, [32]=17, [9]=17},
/*  18 */	{[59]=19, [45]=20, [44]=19, [32]=18, [9 ... 10]=18},
/*  19 */	{},
/*  20 */	{[62]=19},
/*  21 */	{[48 ... 55]=24},
/*  22 */	{[97 ... 102]=25, [65 ... 70]=25, [48 ... 57]=25},
/*  23 */	{[125]=26, [97 ... 122]=23, [95]=23, [65 ... 90]=23, [48 ... 57]=23},
/*  24 */	{[48 ... 55]=27},
/*  25 */	{[97 ... 102]=28, [65 ... 70]=28, [48 ... 57]=28},
/*  26 */	{},
/*  27 */	{},
/*  28 */	{},
};

static uint8_t
fgfx_BEG_REGEX_middle_table[29] = {
	[10]=true,
	[18]=true,
};

static uint8_t
fgfx_BEG_REGEX_final_table[24][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_QUOTE },
	{ 5, 	T_REG_LPAREN },
	{ 6, 	T_REG_RPAREN },
	{ 7, 	T_REG_STAR },
	{ 8, 	T_REG_PLUS },
	{ 9, 	T_REG_DOT },
	{ 10, 	T_REG_LOOK },
	{ 11, 	T_REG_QUES },
	{ 12, 	T_REG_LBRACK },
	{ 13, 	T_REG_CHAR },
	{ 14, 	T_REG_CARET },
	{ 15, 	T_REG_LBRACE },
	{ 16, 	T_REG_UNION },
	{ 19, 	T_CLOSE_REGEX },
	{ 21, 	T_OCT_NUM },
	{ 22, 	T_REG_CHAR },
	{ 24, 	T_OCT_NUM },
	{ 25, 	T_HEX_NUM },
	{ 26, 	T_REG_BOUND_NAME },
	{ 27, 	T_OCT_NUM },
	{ 28, 	T_HEX_NUM },
	{ 0 },
};

static uint8_t
fgfx_BODY_REGEX_state_table[32][256] = {
/*   0 */	{},
/*   1 */	{[125 ... 255]=2, [124]=15, [123]=14, [93 ... 122]=2, [92]=13, [91]=12, [64 ... 90]=2, [63]=11, [48 ... 62]=2, [47]=10, [46]=9, [44 ... 45]=2, [43]=8, [42]=7, [41]=6, [40]=5, [35 ... 39]=2, [34]=4, [33]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{[47]=17, [32]=16, [9]=16},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{[59]=18, [45]=19, [44]=18, [32]=17, [9 ... 10]=17},
/*  11 */	{},
/*  12 */	{},
/*  13 */	{[121 ... 255]=2, [120]=21, [89 ... 119]=2, [88]=21, [56 ... 87]=2, [48 ... 55]=20, [0 ... 47]=2},
/*  14 */	{[97 ... 122]=24, [95]=24, [65 ... 90]=24, [45]=23, [43]=22},
/*  15 */	{},
/*  16 */	{[47]=17, [32]=16, [9]=16},
/*  17 */	{[59]=18, [45]=19, [44]=18, [32]=17, [9 ... 10]=17},
/*  18 */	{},
/*  19 */	{[62]=18},
/*  20 */	{[48 ... 55]=25},
/*  21 */	{[97 ... 102]=26, [65 ... 70]=26, [48 ... 57]=26},
/*  22 */	{[125]=27},
/*  23 */	{[125]=28},
/*  24 */	{[125]=29, [97 ... 122]=24, [95]=24, [65 ... 90]=24, [48 ... 57]=24},
/*  25 */	{[48 ... 55]=30},
/*  26 */	{[97 ... 102]=31, [65 ... 70]=31, [48 ... 57]=31},
/*  27 */	{},
/*  28 */	{},
/*  29 */	{},
/*  30 */	{},
/*  31 */	{},
};

static uint8_t
fgfx_BODY_REGEX_middle_table[32] = {
	[10]=true,
	[17]=true,
};

static uint8_t
fgfx_BODY_REGEX_final_table[25][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_QUOTE },
	{ 5, 	T_REG_LPAREN },
	{ 6, 	T_REG_RPAREN },
	{ 7, 	T_REG_STAR },
	{ 8, 	T_REG_PLUS },
	{ 9, 	T_REG_DOT },
	{ 10, 	T_REG_LOOK },
	{ 11, 	T_REG_QUES },
	{ 12, 	T_REG_LBRACK },
	{ 13, 	T_REG_CHAR },
	{ 14, 	T_REG_LBRACE },
	{ 15, 	T_REG_UNION },
	{ 18, 	T_CLOSE_REGEX },
	{ 20, 	T_OCT_NUM },
	{ 21, 	T_REG_CHAR },
	{ 25, 	T_OCT_NUM },
	{ 26, 	T_HEX_NUM },
	{ 27, 	T_REG_UNION_CLASS },
	{ 28, 	T_REG_DIFF_CLASS },
	{ 29, 	T_REG_BOUND_NAME },
	{ 30, 	T_OCT_NUM },
	{ 31, 	T_HEX_NUM },
	{ 0 },
};

static uint8_t
fgfx_BEG_CCL_state_table[21][256] = {
/*   0 */	{},
/*   1 */	{[95 ... 255]=2, [94]=6, [93]=5, [92]=4, [91]=3, [11 ... 90]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[58]=7},
/*   4 */	{[121 ... 255]=2, [120]=9, [89 ... 119]=2, [88]=9, [56 ... 87]=2, [48 ... 55]=8, [0 ... 47]=2},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{[97 ... 122]=10, [95]=10, [94]=11, [65 ... 90]=10},
/*   8 */	{[48 ... 55]=12},
/*   9 */	{[97 ... 102]=13, [65 ... 70]=13, [48 ... 57]=13},
/*  10 */	{[97 ... 122]=10, [95]=10, [65 ... 90]=10, [58]=14},
/*  11 */	{[97 ... 122]=15, [95]=15, [65 ... 90]=15},
/*  12 */	{[48 ... 55]=16},
/*  13 */	{[97 ... 102]=17, [65 ... 70]=17, [48 ... 57]=17},
/*  14 */	{[93]=18},
/*  15 */	{[97 ... 122]=15, [95]=15, [65 ... 90]=15, [58]=19},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{[93]=20},
/*  20 */	{},
};

static uint8_t
fgfx_BEG_CCL_final_table[14][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_RBRACK },
	{ 6, 	T_REG_CARET },
	{ 8, 	T_OCT_NUM },
	{ 9, 	T_REG_CHAR },
	{ 12, 	T_OCT_NUM },
	{ 13, 	T_HEX_NUM },
	{ 16, 	T_OCT_NUM },
	{ 17, 	T_HEX_NUM },
	{ 18, 	T_CCE },
	{ 20, 	T_N_CCE },
	{ 0 },
};

static uint8_t
fgfx_BODY_CCL_state_table[22][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=6, [92]=5, [91]=4, [46 ... 90]=2, [45]=3, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[94 ... 255]=7, [11 ... 92]=7, [0 ... 9]=7},
/*   4 */	{[58]=8},
/*   5 */	{[121 ... 255]=2, [120]=10, [89 ... 119]=2, [88]=10, [56 ... 87]=2, [48 ... 55]=9, [0 ... 47]=2},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{[97 ... 122]=11, [95]=11, [94]=12, [65 ... 90]=11},
/*   9 */	{[48 ... 55]=13},
/*  10 */	{[97 ... 102]=14, [65 ... 70]=14, [48 ... 57]=14},
/*  11 */	{[97 ... 122]=11, [95]=11, [65 ... 90]=11, [58]=15},
/*  12 */	{[97 ... 122]=16, [95]=16, [65 ... 90]=16},
/*  13 */	{[48 ... 55]=17},
/*  14 */	{[97 ... 102]=18, [65 ... 70]=18, [48 ... 57]=18},
/*  15 */	{[93]=19},
/*  16 */	{[97 ... 122]=16, [95]=16, [65 ... 90]=16, [58]=20},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{[93]=21},
/*  21 */	{},
};

static uint8_t
fgfx_BODY_CCL_middle_table[22] = {
	[3]=true,
};

static uint8_t
fgfx_BODY_CCL_final_table[15][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REG_RBRACK },
	{ 7, 	T_REG_HYPHEN },
	{ 9, 	T_OCT_NUM },
	{ 10, 	T_REG_CHAR },
	{ 13, 	T_OCT_NUM },
	{ 14, 	T_HEX_NUM },
	{ 17, 	T_OCT_NUM },
	{ 18, 	T_HEX_NUM },
	{ 19, 	T_CCE },
	{ 21, 	T_N_CCE },
	{ 0 },
};

static uint8_t
fgfx_STRING_state_table[11][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=4, [35 ... 91]=2, [34]=3, [11 ... 33]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[121 ... 255]=2, [120]=6, [89 ... 119]=2, [88]=6, [56 ... 87]=2, [48 ... 55]=5, [0 ... 47]=2},
/*   5 */	{[48 ... 55]=7},
/*   6 */	{[97 ... 102]=8, [65 ... 70]=8, [48 ... 57]=8},
/*   7 */	{[48 ... 55]=9},
/*   8 */	{[97 ... 102]=10, [65 ... 70]=10, [48 ... 57]=10},
/*   9 */	{},
/*  10 */	{},
};

static uint8_t
fgfx_STRING_final_table[10][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_OCT_NUM },
	{ 6, 	T_REG_CHAR },
	{ 7, 	T_OCT_NUM },
	{ 8, 	T_HEX_NUM },
	{ 9, 	T_OCT_NUM },
	{ 10, 	T_HEX_NUM },
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
fgfx_look_table[3] = {
	T_CLOSE_REGEX,
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
