#ifndef FGFX_LEX_H
#define FGFX_LEX_H

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
	TTOKEN,
	TSEMI,
	TCOMMA,
	TSPACE,
	TCOM,
	TEOF,
};

#define TOTAL_TOKEN	14

#ifndef ONLY_TOKEN

#define START_STATE	1
#define DEAD_STATE	0

//Size of state table = 32
static dfa_state_t fgfx_state_table[][256] = {
/*   0 */	{},
/*   1 */	{[124]=12, [97 ... 122]=11, [95]=11, [65 ... 90]=11, [64]=10, [61]=9, [60]=8, [59]=7, [47]=6, [45]=5, [44]=4, [37]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[115]=14, [105]=13},
/*   4 */	{},
/*   5 */	{[62]=15},
/*   6 */	{[47]=17, [42]=16},
/*   7 */	{},
/*   8 */	{[97 ... 122]=18, [95]=18, [65 ... 90]=18},
/*   9 */	{},
/*  10 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19},
/*  11 */	{[97 ... 122]=11, [95]=11, [65 ... 90]=11, [48 ... 57]=11},
/*  12 */	{},
/*  13 */	{[103]=20},
/*  14 */	{[107]=21},
/*  15 */	{},
/*  16 */	{[43 ... 255]=16, [42]=22, [0 ... 41]=16},
/*  17 */	{[11 ... 255]=17, [0 ... 9]=17},
/*  18 */	{[97 ... 122]=18, [95]=18, [65 ... 90]=18, [62]=24, [48 ... 57]=18, [39]=23},
/*  19 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19, [48 ... 57]=19},
/*  20 */	{[99]=25},
/*  21 */	{[105]=26},
/*  22 */	{[48 ... 255]=16, [47]=27, [43 ... 46]=16, [42]=22, [0 ... 41]=16},
/*  23 */	{[62]=24, [39]=23},
/*  24 */	{},
/*  25 */	{[97]=28},
/*  26 */	{[112]=29},
/*  27 */	{},
/*  28 */	{[115]=30},
/*  29 */	{},
/*  30 */	{[101]=31},
/*  31 */	{},
};

static final_state_t fgfx_final_table[][2] = {
	{ 2, 	TSPACE },
	{ 4, 	TCOMMA },
	{ 7, 	TSEMI },
	{ 9, 	TEQUAL },
	{ 11, 	TTOKEN },
	{ 12, 	TUNION },
	{ 15, 	TARROW },
	{ 17, 	TCOM },
	{ 19, 	TLOCAL_TOK },
	{ 24, 	TNON_TER },
	{ 27, 	TCOM },
	{ 29, 	TSKIP },
	{ 31, 	TIGCASE },
};

#define SIZE_FINAL_TAB	13

static int fgfx_skip_table[] = {
	TSPACE,
	TCOM,
	-1,
};

#endif /* ONLY_TOKEN */
#endif /* FGFX_LEX_H */