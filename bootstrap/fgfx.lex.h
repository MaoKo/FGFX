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
	S_REG_PARAMS,
	S_REG_NO_PARAMS,
	S_REG_COM,
	S_NESTED_COM,
};

#define TOTAL_STATE	12
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
	T_REG_TILDE,
	T_REG_LBRACE,
	T_REG_RBRACE,
	T_DIGIT,
	T_REG_COMMA,
	T_REG_DIFF_CLASS,
	T_REG_UNION_CLASS,
	T_REG_INTER_CLASS,
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
	T_REG_OPTION,
	T_REG_INVERT,
	T_IGCASE,
	T_DOTALL,
	T_SKIPWS,
	T_REVERSE,
	T_REG_COLON,
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
	T_SINGLE_LINE,
	T_BEG_MULTI,
	T_END_MULTI,
	T_CHAR_COMMENT,
	T_MULTI_LINE,
	T_REG_BEG_COM,
	T_REG_END_COM,
	T_REG_CHAR_COM,
	T_REG_NOTHING,
	T_SKIP,
	T_TOKEN,
	T_KEYWORD,
	T_STATE,
	T_FRAGMENT,
	T_INITIAL,
	T_STAY,
	T_BEGIN,
	T_PUSH,
	T_POP,
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

#define TOTAL_TOKEN	113

#if 0

static char const*
fgfx_token_name_table[113] = {
	"EQUAL",
	"STAR",
	"OPEN_REGEX",
	"CLOSE_REGEX",
	"REG_UNION",
	"REG_STAR",
	"REG_PLUS",
	"REG_QUES",
	"REG_LPAREN",
	"REG_RPAREN",
	"REG_DOT",
	"REG_LOOK",
	"REG_QUOTE",
	"REG_BOUND_NAME",
	"REG_TILDE",
	"REG_LBRACE",
	"REG_RBRACE",
	"DIGIT",
	"REG_COMMA",
	"REG_DIFF_CLASS",
	"REG_UNION_CLASS",
	"REG_INTER_CLASS",
	"REG_LBRACK",
	"REG_RBRACK",
	"REG_CARET",
	"REG_DOLLAR",
	"REG_HYPHEN",
	"CCE",
	"N_CCE",
	"OCT_NUM",
	"HEX_NUM",
	"REG_CHAR",
	"REG_OPTION",
	"REG_INVERT",
	"IGCASE",
	"DOTALL",
	"SKIPWS",
	"REVERSE",
	"REG_COLON",
	"NON_TERMINAL",
	"LITERAL",
	"UNION",
	"LBRACK",
	"RBRACK",
	"DIRECTIVE",
	"TERMINAL",
	"ARROW",
	"BARROW",
	"SEMI",
	"COMMA",
	"LBRACE",
	"RBRACE",
	"LPAREN",
	"RPAREN",
	"SPACE",
	"SINGLE_LINE",
	"BEG_MULTI",
	"END_MULTI",
	"CHAR_COMMENT",
	"MULTI_LINE",
	"REG_BEG_COM",
	"REG_END_COM",
	"REG_CHAR_COM",
	"REG_NOTHING",
	"SKIP",
	"TOKEN",
	"KEYWORD",
	"STATE",
	"FRAGMENT",
	"INITIAL",
	"STAY",
	"BEGIN",
	"PUSH",
	"POP",
	"CC_FIRST",
	"CCE_ALNUM",
	"CCE_ALPHA",
	"CCE_CNTRL",
	"CCE_DIGIT",
	"CCE_GRAPH",
	"CCE_LOWER",
	"CCE_PRINT",
	"CCE_PUNCT",
	"CCE_SPACE",
	"CCE_BLANK",
	"CCE_UPPER",
	"CCE_XDIGIT",
	"CC_MIDDLE",
	"N_CCE_ALNUM",
	"N_CCE_ALPHA",
	"N_CCE_CNTRL",
	"N_CCE_DIGIT",
	"N_CCE_GRAPH",
	"N_CCE_LOWER",
	"N_CCE_PRINT",
	"N_CCE_PUNCT",
	"N_CCE_SPACE",
	"N_CCE_BLANK",
	"N_CCE_UPPER",
	"N_CCE_XDIGIT",
	"CC_LAST",
	"EXTERN",
	"PRODUCTION",
	"ALIAS",
	"PRECEDENCE",
	"MIMIC",
	"EMPTY",
	"START",
	"LEFT",
	"RIGHT",
	"NONASSOC",
	"ERROR",
	"EOF",
};

#endif

#ifndef _ONLY_STATE_TOKEN_

enum {
	_BEGIN	= 0x1000,
	_PUSH	= 0x2000,
	POP	= 0x4000,
};

#define BEGIN(x)	(x | _BEGIN)
#define PUSH(x)	(x | _PUSH)

static uint16_t
fgfx_action_table[TOTAL_TOKEN][TOTAL_STATE] = {
	[T_OPEN_REGEX][S_GLOBAL] = BEGIN(S_BEG_REGEX), 
	[T_CLOSE_REGEX][S_BODY_REGEX] = BEGIN(S_GLOBAL), 
	[T_REG_LPAREN][S_BODY_REGEX] = PUSH(S_BODY_REGEX), 
	[T_REG_RPAREN][S_BODY_REGEX] = POP, 
	[T_REG_QUOTE][S_BODY_REGEX] = BEGIN(S_STRING), [T_REG_QUOTE][S_STRING] = BEGIN(S_BODY_REGEX), 
	[T_REG_LBRACE][S_BODY_REGEX] = BEGIN(S_FINITE_SEQ), 
	[T_REG_RBRACE][S_FINITE_SEQ] = BEGIN(S_BODY_REGEX), 
	[T_REG_LBRACK][S_BODY_REGEX] = BEGIN(S_BEG_CCL), 
	[T_REG_RBRACK][S_BODY_CCL] = BEGIN(S_BODY_REGEX), 
	[T_REG_CARET][S_BEG_REGEX] = BEGIN(S_BODY_REGEX), [T_REG_CARET][S_BEG_CCL] = BEGIN(S_BODY_CCL), 
	[T_REG_OPTION][S_BODY_REGEX] = PUSH(S_REG_PARAMS), 
	[T_REG_INVERT][S_REG_PARAMS] = BEGIN(S_REG_NO_PARAMS), 
	[T_REG_COLON][S_REG_PARAMS] = BEGIN(S_BODY_REGEX), [T_REG_COLON][S_REG_NO_PARAMS] = BEGIN(S_BODY_REGEX), 
	[T_BEG_MULTI][S_GLOBAL] = PUSH(S_NESTED_COM), [T_BEG_MULTI][S_NESTED_COM] = PUSH(S_NESTED_COM), 
	[T_END_MULTI][S_NESTED_COM] = POP, 
	[T_REG_BEG_COM][S_BEG_REGEX] = PUSH(S_REG_COM), [T_REG_BEG_COM][S_BODY_REGEX] = PUSH(S_REG_COM), 
	[T_REG_END_COM][S_REG_COM] = POP, 
	[T_REG_NOTHING][S_BEG_REGEX] = BEGIN(S_BODY_REGEX), [T_REG_NOTHING][S_BEG_CCL] = BEGIN(S_BODY_CCL), 
};

#define START_STATE	1
#define DEAD_STATE	0

static uint8_t
fgfx_GLOBAL_state_table[49][256] = {
/*   0 */	{},
/*   1 */	{[125]=19, [124]=18, [123]=17, [97 ... 122]=14, [95]=14, [93]=16, [91]=15, [65 ... 90]=14, [61]=13, [60]=12, [59]=11, [47]=10, [45]=9, [44]=8, [42]=7, [41]=6, [40]=5, [39]=4, [36]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=20, [9 ... 10]=20},
/*   3 */	{[97 ... 122]=21, [95]=21, [65 ... 90]=21},
/*   4 */	{[93 ... 255]=22, [92]=23, [40 ... 91]=22, [35 ... 38]=22, [11 ... 33]=22, [0 ... 9]=22},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{[62]=24},
/*  10 */	{[47]=27, [42]=26, [32]=25, [9]=25},
/*  11 */	{},
/*  12 */	{[97 ... 122]=28, [95]=28, [65 ... 90]=28},
/*  13 */	{[62]=29},
/*  14 */	{[97 ... 122]=31, [95]=31, [65 ... 90]=31, [48 ... 57]=30},
/*  15 */	{},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{},
/*  19 */	{},
/*  20 */	{[32]=20, [9 ... 10]=20},
/*  21 */	{[97 ... 122]=33, [95]=33, [65 ... 90]=33, [48 ... 57]=32},
/*  22 */	{[93 ... 255]=34, [92]=36, [40 ... 91]=34, [39]=35, [35 ... 38]=34, [11 ... 33]=34, [0 ... 9]=34},
/*  23 */	{[92]=37, [39]=37, [34]=37},
/*  24 */	{},
/*  25 */	{[32]=25, [9]=25},
/*  26 */	{},
/*  27 */	{[11 ... 255]=38, [0 ... 9]=38},
/*  28 */	{[97 ... 122]=42, [95]=42, [65 ... 90]=42, [62]=41, [48 ... 57]=40, [39]=39},
/*  29 */	{},
/*  30 */	{[97 ... 122]=31, [95]=31, [65 ... 90]=31, [48 ... 57]=43},
/*  31 */	{[97 ... 122]=31, [95]=31, [65 ... 90]=31, [48 ... 57]=30},
/*  32 */	{[97 ... 122]=33, [95]=33, [65 ... 90]=33, [48 ... 57]=44},
/*  33 */	{[97 ... 122]=33, [95]=33, [65 ... 90]=33, [48 ... 57]=32},
/*  34 */	{[93 ... 255]=34, [92]=36, [40 ... 91]=34, [39]=35, [35 ... 38]=34, [11 ... 33]=34, [0 ... 9]=34},
/*  35 */	{},
/*  36 */	{[92]=45, [39]=45, [34]=45},
/*  37 */	{[93 ... 255]=34, [92]=36, [40 ... 91]=34, [39]=35, [35 ... 38]=34, [11 ... 33]=34, [0 ... 9]=34},
/*  38 */	{[11 ... 255]=38, [0 ... 9]=38},
/*  39 */	{[62]=41, [39]=46},
/*  40 */	{[97 ... 122]=42, [95]=42, [65 ... 90]=42, [62]=41, [48 ... 57]=47, [39]=39},
/*  41 */	{},
/*  42 */	{[97 ... 122]=42, [95]=42, [65 ... 90]=42, [62]=41, [48 ... 57]=40, [39]=39},
/*  43 */	{[97 ... 122]=31, [95]=31, [65 ... 90]=31, [48 ... 57]=43},
/*  44 */	{[97 ... 122]=33, [95]=33, [65 ... 90]=33, [48 ... 57]=44},
/*  45 */	{[93 ... 255]=34, [92]=36, [40 ... 91]=34, [39]=35, [35 ... 38]=34, [11 ... 33]=34, [0 ... 9]=34},
/*  46 */	{[62]=41, [39]=48},
/*  47 */	{[97 ... 122]=42, [95]=42, [65 ... 90]=42, [62]=41, [48 ... 57]=47, [39]=39},
/*  48 */	{[62]=41},
};

static uint8_t
fgfx_GLOBAL_final_table[31][2] = {
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
	{ 20, 	T_SPACE },
	{ 21, 	T_DIRECTIVE },
	{ 24, 	T_ARROW },
	{ 25, 	T_OPEN_REGEX },
	{ 26, 	T_BEG_MULTI },
	{ 27, 	T_SINGLE_LINE },
	{ 29, 	T_BARROW },
	{ 30, 	T_TERMINAL },
	{ 31, 	T_TERMINAL },
	{ 32, 	T_DIRECTIVE },
	{ 33, 	T_DIRECTIVE },
	{ 35, 	T_LITERAL },
	{ 38, 	T_SINGLE_LINE },
	{ 41, 	T_NON_TERMINAL },
	{ 43, 	T_TERMINAL },
	{ 44, 	T_DIRECTIVE },
	{ 0 },
};

static uint8_t
fgfx_BEG_REGEX_state_table[6][256] = {
/*   0 */	{},
/*   1 */	{[94]=3, [40]=2},
/*   2 */	{[63]=4},
/*   3 */	{},
/*   4 */	{[35]=5},
/*   5 */	{},
};

static uint8_t
fgfx_BEG_REGEX_final_table[4][2] = {
	{ 1, 	T_REG_NOTHING },
	{ 3, 	T_REG_CARET },
	{ 5, 	T_REG_BEG_COM },
	{ 0 },
};

static uint8_t
fgfx_BODY_REGEX_state_table[58][256] = {
/*   0 */	{},
/*   1 */	{[127 ... 255]=2, [126]=17, [125]=2, [124]=16, [123]=15, [93 ... 122]=2, [92]=14, [91]=13, [64 ... 90]=2, [63]=12, [48 ... 62]=2, [47]=11, [46]=10, [44 ... 45]=2, [43]=9, [42]=8, [41]=7, [40]=6, [37 ... 39]=2, [36]=5, [35]=2, [34]=4, [33]=2, [32]=3, [11 ... 31]=2, [9]=3, [0 ... 8]=2},
/*   2 */	{},
/*   3 */	{[47]=19, [32]=18, [9]=18},
/*   4 */	{},
/*   5 */	{[47]=21, [32]=20, [9]=20},
/*   6 */	{[63]=22},
/*   7 */	{},
/*   8 */	{},
/*   9 */	{},
/*  10 */	{},
/*  11 */	{[59]=26, [45]=25, [44]=24, [32]=23, [9 ... 10]=23},
/*  12 */	{},
/*  13 */	{},
/*  14 */	{[121 ... 255]=2, [120]=29, [89 ... 119]=2, [88]=29, [56 ... 87]=2, [48 ... 55]=28, [11 ... 47]=2, [10]=27, [0 ... 9]=2},
/*  15 */	{[97 ... 122]=33, [95]=33, [65 ... 90]=33, [45]=32, [43]=31, [38]=30},
/*  16 */	{},
/*  17 */	{},
/*  18 */	{[47]=19, [32]=18, [9]=18},
/*  19 */	{[59]=26, [45]=25, [44]=24, [32]=23, [9 ... 10]=23},
/*  20 */	{[47]=21, [32]=20, [9]=20},
/*  21 */	{[59]=37, [45]=36, [44]=35, [32]=34, [9 ... 10]=34},
/*  22 */	{[35]=38},
/*  23 */	{[59]=26, [45]=25, [44]=24, [32]=39, [9 ... 10]=39},
/*  24 */	{},
/*  25 */	{[62]=40},
/*  26 */	{},
/*  27 */	{[92]=42, [32]=41, [9]=41},
/*  28 */	{[48 ... 55]=43},
/*  29 */	{[97 ... 102]=44, [65 ... 70]=44, [48 ... 57]=44},
/*  30 */	{[125]=45},
/*  31 */	{[125]=46},
/*  32 */	{[125]=47},
/*  33 */	{[125]=50, [97 ... 122]=49, [95]=49, [65 ... 90]=49, [48 ... 57]=48},
/*  34 */	{[59]=37, [45]=36, [44]=35, [32]=51, [9 ... 10]=51},
/*  35 */	{},
/*  36 */	{[62]=52},
/*  37 */	{},
/*  38 */	{},
/*  39 */	{[59]=26, [45]=25, [44]=24, [32]=39, [9 ... 10]=39},
/*  40 */	{},
/*  41 */	{[92]=42, [32]=41, [9]=41},
/*  42 */	{[10]=53},
/*  43 */	{[48 ... 55]=54},
/*  44 */	{[97 ... 102]=55, [65 ... 70]=55, [48 ... 57]=55},
/*  45 */	{},
/*  46 */	{},
/*  47 */	{},
/*  48 */	{[125]=50, [97 ... 122]=49, [95]=49, [65 ... 90]=49, [48 ... 57]=56},
/*  49 */	{[125]=50, [97 ... 122]=49, [95]=49, [65 ... 90]=49, [48 ... 57]=48},
/*  50 */	{},
/*  51 */	{[59]=37, [45]=36, [44]=35, [32]=51, [9 ... 10]=51},
/*  52 */	{},
/*  53 */	{[92]=42, [32]=57, [9]=57},
/*  54 */	{},
/*  55 */	{},
/*  56 */	{[125]=50, [97 ... 122]=49, [95]=49, [65 ... 90]=49, [48 ... 57]=56},
/*  57 */	{[92]=42, [32]=57, [9]=57},
};

static uint8_t
fgfx_BODY_REGEX_ahead_table[58][2] = {
	[20] = { true, false, },
	[21] = { true, false, },
	[23] = { true, false, },
	[24] = { true, false, },
	[25] = { true, false, },
	[26] = { true, false, },
	[34] = { true, false, },
	[35] = { true, false, },
	[36] = { true, false, },
	[37] = { true, false, },
	[39] = { true, false, },
	[51] = { true, false, },
};

static uint8_t
fgfx_BODY_REGEX_final_table[39][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_QUOTE },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REG_LPAREN },
	{ 7, 	T_REG_RPAREN },
	{ 8, 	T_REG_STAR },
	{ 9, 	T_REG_PLUS },
	{ 10, 	T_REG_DOT },
	{ 11, 	T_REG_LOOK },
	{ 12, 	T_REG_QUES },
	{ 13, 	T_REG_LBRACK },
	{ 14, 	T_REG_CHAR },
	{ 15, 	T_REG_LBRACE },
	{ 16, 	T_REG_UNION },
	{ 17, 	T_REG_TILDE },
	{ 22, 	T_REG_OPTION },
	{ 24, 	T_CLOSE_REGEX },
	{ 26, 	T_CLOSE_REGEX },
	{ 27, 	T_MULTI_LINE },
	{ 28, 	T_OCT_NUM },
	{ 29, 	T_REG_CHAR },
	{ 35, 	T_REG_DOLLAR },
	{ 37, 	T_REG_DOLLAR },
	{ 38, 	T_REG_BEG_COM },
	{ 40, 	T_CLOSE_REGEX },
	{ 41, 	T_MULTI_LINE },
	{ 43, 	T_OCT_NUM },
	{ 44, 	T_HEX_NUM },
	{ 45, 	T_REG_INTER_CLASS },
	{ 46, 	T_REG_UNION_CLASS },
	{ 47, 	T_REG_DIFF_CLASS },
	{ 50, 	T_REG_BOUND_NAME },
	{ 52, 	T_REG_DOLLAR },
	{ 53, 	T_MULTI_LINE },
	{ 54, 	T_OCT_NUM },
	{ 55, 	T_HEX_NUM },
	{ 57, 	T_MULTI_LINE },
	{ 0 },
};

static uint8_t
fgfx_BEG_CCL_state_table[3][256] = {
/*   0 */	{},
/*   1 */	{[94]=2},
/*   2 */	{},
};

static uint8_t
fgfx_BEG_CCL_final_table[3][2] = {
	{ 1, 	T_REG_NOTHING },
	{ 2, 	T_REG_CARET },
	{ 0 },
};

static uint8_t
fgfx_BODY_CCL_state_table[29][256] = {
/*   0 */	{},
/*   1 */	{[94 ... 255]=2, [93]=6, [92]=5, [91]=4, [46 ... 90]=2, [45]=3, [11 ... 44]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{[94 ... 255]=7, [11 ... 92]=7, [0 ... 9]=7},
/*   4 */	{[58]=8},
/*   5 */	{[121 ... 255]=2, [120]=11, [89 ... 119]=2, [88]=11, [56 ... 87]=2, [48 ... 55]=10, [11 ... 47]=2, [10]=9, [0 ... 9]=2},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{[97 ... 122]=12, [95]=12, [94]=13, [65 ... 90]=12},
/*   9 */	{[92]=15, [32]=14, [9]=14},
/*  10 */	{[48 ... 55]=16},
/*  11 */	{[97 ... 102]=17, [65 ... 70]=17, [48 ... 57]=17},
/*  12 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19, [58]=18},
/*  13 */	{[97 ... 122]=20, [95]=20, [65 ... 90]=20},
/*  14 */	{[92]=15, [32]=14, [9]=14},
/*  15 */	{[10]=21},
/*  16 */	{[48 ... 55]=22},
/*  17 */	{[97 ... 102]=23, [65 ... 70]=23, [48 ... 57]=23},
/*  18 */	{[93]=24},
/*  19 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19, [58]=18},
/*  20 */	{[97 ... 122]=26, [95]=26, [65 ... 90]=26, [58]=25},
/*  21 */	{[92]=15, [32]=27, [9]=27},
/*  22 */	{},
/*  23 */	{},
/*  24 */	{},
/*  25 */	{[93]=28},
/*  26 */	{[97 ... 122]=26, [95]=26, [65 ... 90]=26, [58]=25},
/*  27 */	{[92]=15, [32]=27, [9]=27},
/*  28 */	{},
};

static uint8_t
fgfx_BODY_CCL_ahead_table[29][2] = {
	[7] = { true, false, },
};

static uint8_t
fgfx_BODY_CCL_final_table[19][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_CHAR },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_REG_CHAR },
	{ 6, 	T_REG_RBRACK },
	{ 7, 	T_REG_HYPHEN },
	{ 9, 	T_MULTI_LINE },
	{ 10, 	T_OCT_NUM },
	{ 11, 	T_REG_CHAR },
	{ 14, 	T_MULTI_LINE },
	{ 16, 	T_OCT_NUM },
	{ 17, 	T_HEX_NUM },
	{ 21, 	T_MULTI_LINE },
	{ 22, 	T_OCT_NUM },
	{ 23, 	T_HEX_NUM },
	{ 24, 	T_CCE },
	{ 27, 	T_MULTI_LINE },
	{ 28, 	T_N_CCE },
	{ 0 },
};

static uint8_t
fgfx_STRING_state_table[16][256] = {
/*   0 */	{},
/*   1 */	{[93 ... 255]=2, [92]=4, [35 ... 91]=2, [34]=3, [11 ... 33]=2, [0 ... 9]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{[121 ... 255]=2, [120]=7, [89 ... 119]=2, [88]=7, [56 ... 87]=2, [48 ... 55]=6, [11 ... 47]=2, [10]=5, [0 ... 9]=2},
/*   5 */	{[92]=9, [32]=8, [9]=8},
/*   6 */	{[48 ... 55]=10},
/*   7 */	{[97 ... 102]=11, [65 ... 70]=11, [48 ... 57]=11},
/*   8 */	{[92]=9, [32]=8, [9]=8},
/*   9 */	{[10]=12},
/*  10 */	{[48 ... 55]=13},
/*  11 */	{[97 ... 102]=14, [65 ... 70]=14, [48 ... 57]=14},
/*  12 */	{[92]=9, [32]=15, [9]=15},
/*  13 */	{},
/*  14 */	{},
/*  15 */	{[92]=9, [32]=15, [9]=15},
};

static uint8_t
fgfx_STRING_final_table[14][2] = {
	{ 2, 	T_REG_CHAR },
	{ 3, 	T_REG_QUOTE },
	{ 4, 	T_REG_CHAR },
	{ 5, 	T_MULTI_LINE },
	{ 6, 	T_OCT_NUM },
	{ 7, 	T_REG_CHAR },
	{ 8, 	T_MULTI_LINE },
	{ 10, 	T_OCT_NUM },
	{ 11, 	T_HEX_NUM },
	{ 12, 	T_MULTI_LINE },
	{ 13, 	T_OCT_NUM },
	{ 14, 	T_HEX_NUM },
	{ 15, 	T_MULTI_LINE },
	{ 0 },
};

static uint8_t
fgfx_FINITE_SEQ_state_table[6][256] = {
/*   0 */	{},
/*   1 */	{[125]=4, [48 ... 57]=3, [44]=2},
/*   2 */	{},
/*   3 */	{[48 ... 57]=5},
/*   4 */	{},
/*   5 */	{[48 ... 57]=5},
};

static uint8_t
fgfx_FINITE_SEQ_final_table[5][2] = {
	{ 2, 	T_REG_COMMA },
	{ 3, 	T_DIGIT },
	{ 4, 	T_REG_RBRACE },
	{ 5, 	T_DIGIT },
	{ 0 },
};

static uint8_t
fgfx_REG_PARAMS_state_table[8][256] = {
/*   0 */	{},
/*   1 */	{[120]=7, [115]=6, [114]=5, [105]=4, [58]=3, [45]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
};

static uint8_t
fgfx_REG_PARAMS_final_table[7][2] = {
	{ 2, 	T_REG_INVERT },
	{ 3, 	T_REG_COLON },
	{ 4, 	T_IGCASE },
	{ 5, 	T_REVERSE },
	{ 6, 	T_DOTALL },
	{ 7, 	T_SKIPWS },
	{ 0 },
};

static uint8_t
fgfx_REG_NO_PARAMS_state_table[7][256] = {
/*   0 */	{},
/*   1 */	{[120]=6, [115]=5, [114]=4, [105]=3, [58]=2},
/*   2 */	{},
/*   3 */	{},
/*   4 */	{},
/*   5 */	{},
/*   6 */	{},
};

static uint8_t
fgfx_REG_NO_PARAMS_final_table[6][2] = {
	{ 2, 	T_REG_COLON },
	{ 3, 	T_IGCASE },
	{ 4, 	T_REVERSE },
	{ 5, 	T_DOTALL },
	{ 6, 	T_SKIPWS },
	{ 0 },
};

static uint8_t
fgfx_REG_COM_state_table[5][256] = {
/*   0 */	{},
/*   1 */	{[42 ... 255]=2, [41]=3, [0 ... 40]=2},
/*   2 */	{[42 ... 255]=4, [0 ... 40]=4},
/*   3 */	{},
/*   4 */	{[42 ... 255]=4, [0 ... 40]=4},
};

static uint8_t
fgfx_REG_COM_final_table[4][2] = {
	{ 2, 	T_REG_CHAR_COM },
	{ 3, 	T_REG_END_COM },
	{ 4, 	T_REG_CHAR_COM },
	{ 0 },
};

static uint8_t
fgfx_NESTED_COM_state_table[8][256] = {
/*   0 */	{},
/*   1 */	{[48 ... 255]=2, [47]=4, [43 ... 46]=2, [42]=3, [0 ... 41]=2},
/*   2 */	{},
/*   3 */	{[47]=6, [42]=5},
/*   4 */	{[42]=7},
/*   5 */	{[47]=6, [42]=5},
/*   6 */	{},
/*   7 */	{},
};

static uint8_t
fgfx_NESTED_COM_final_table[6][2] = {
	{ 2, 	T_CHAR_COMMENT },
	{ 3, 	T_CHAR_COMMENT },
	{ 4, 	T_CHAR_COMMENT },
	{ 6, 	T_END_MULTI },
	{ 7, 	T_BEG_MULTI },
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
fgfx_skip_table[11] = {
	T_SPACE,
	T_SINGLE_LINE,
	T_BEG_MULTI,
	T_END_MULTI,
	T_CHAR_COMMENT,
	T_MULTI_LINE,
	T_REG_BEG_COM,
	T_REG_END_COM,
	T_REG_CHAR_COM,
	T_REG_NOTHING,
	-1,
};

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _FGFX_LEX_H_ */
