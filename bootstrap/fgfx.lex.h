#ifndef _FGFX_H
#define _FGFX_H

#include <stdint.h>
typedef	uint8_t	dfa_state_t;
typedef	uint8_t	final_state_t;

enum {
	TNONE,
	TLOCAL_TOK,
	TEQUAL,
	TIGCASE,
	TSKIP,
	TNON_TER,
	TARROW,
	TUNION,
	TSTR,
	TSTART,
	TTOKEN_FILE,
	TTOKEN,
	TSEMI,
	TCOMMA,
	TSPACE,
	TCOM,
	TEOF,
};

#define TOTAL_TOKEN	17

#ifndef ONLY_TOKEN

#define START_STATE	1
#define DEAD_STATE	0

static dfa_state_t fgfx_state_table[49][256] = {
/*   0 */	{},
/*   1 */	{[124]=13, [97 ... 122]=12, [95]=12, [65 ... 90]=12, [64]=11, [61]=10, [60]=9, [59]=8, [47]=7, [45]=6, [44]=5, [37]=4, [34]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[35 ... 255]=14, [11 ... 33]=14, [0 ... 9]=14},
/*   4 */	{[116]=17, [115]=16, [105]=15},
/*   5 */	{},
/*   6 */	{[62]=18},
/*   7 */	{[47]=20, [42]=19},
/*   8 */	{},
/*   9 */	{[97 ... 122]=21, [95]=21, [65 ... 90]=21},
/*  10 */	{},
/*  11 */	{[97 ... 122]=22, [95]=22, [65 ... 90]=22},
/*  12 */	{[97 ... 122]=12, [95]=12, [65 ... 90]=12, [48 ... 57]=12},
/*  13 */	{},
/*  14 */	{[35 ... 255]=14, [34]=23, [11 ... 33]=14, [0 ... 9]=14},
/*  15 */	{[103]=24},
/*  16 */	{[116]=26, [107]=25},
/*  17 */	{[111]=27},
/*  18 */	{},
/*  19 */	{[43 ... 255]=19, [42]=28, [0 ... 41]=19},
/*  20 */	{[11 ... 255]=20, [0 ... 9]=20},
/*  21 */	{[97 ... 122]=21, [95]=21, [65 ... 90]=21, [62]=30, [48 ... 57]=21, [39]=29},
/*  22 */	{[97 ... 122]=22, [95]=22, [65 ... 90]=22, [48 ... 57]=22},
/*  23 */	{},
/*  24 */	{[99]=31},
/*  25 */	{[105]=32},
/*  26 */	{[97]=33},
/*  27 */	{[107]=34},
/*  28 */	{[48 ... 255]=19, [47]=35, [43 ... 46]=19, [42]=28, [0 ... 41]=19},
/*  29 */	{[62]=30},
/*  30 */	{},
/*  31 */	{[97]=36},
/*  32 */	{[112]=37},
/*  33 */	{[114]=38},
/*  34 */	{[101]=39},
/*  35 */	{},
/*  36 */	{[115]=40},
/*  37 */	{},
/*  38 */	{[116]=41},
/*  39 */	{[110]=42},
/*  40 */	{[101]=43},
/*  41 */	{},
/*  42 */	{[95]=44},
/*  43 */	{},
/*  44 */	{[102]=45},
/*  45 */	{[105]=46},
/*  46 */	{[108]=47},
/*  47 */	{[101]=48},
/*  48 */	{},
};

static final_state_t fgfx_final_table[][2] = {
	{ 2, 	TSPACE },
	{ 5, 	TCOMMA },
	{ 8, 	TSEMI },
	{ 10, 	TEQUAL },
	{ 12, 	TTOKEN },
	{ 13, 	TUNION },
	{ 18, 	TARROW },
	{ 20, 	TCOM },
	{ 22, 	TLOCAL_TOK },
	{ 23, 	TSTR },
	{ 30, 	TNON_TER },
	{ 35, 	TCOM },
	{ 37, 	TSKIP },
	{ 41, 	TSTART },
	{ 43, 	TIGCASE },
	{ 48, 	TTOKEN_FILE },
};

#define SIZE_FINAL_TAB	16

static int fgfx_skip_table[] = {
	TSPACE,
	TCOM,
	-1,
};

#endif /* ONLY_TOKEN */
#endif /* _FGFX_H */