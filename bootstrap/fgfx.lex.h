#ifndef FGFX_LEX_H
#define FGFX_LEX_H

#include <stdint.h>
typedef	uint8_t	dfa_state_t;
typedef	uint8_t	final_state_t;

enum {
	TNONE,
	TG_IDENT,
	TL_IDENT,
	TEQUAL,
	TIGCASE,
	TSKIP,
	TTER,
	TNON_TER,
	TENON_TER,
	TARROW,
	TUNION,
	TSEMI,
	TCOMMA,
	TSPACE,
	TCOM,
	TEOF,
};

#define TOTAL_TOKEN	16

#ifndef ONLY_TOKEN

#define START_STATE	1
#define DEAD_STATE	0

//Size of state table = 33
static dfa_state_t fgfx_state_table[][256] = {
/*   0 */	{},
/*   1 */	{[124]=13, [97 ... 122]=12, [95]=12, [65 ... 90]=12, [64]=11, [61]=10, [60]=9, [59]=8, [47]=7, [45]=6, [44]=5, [37]=4, [35]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[11 ... 255]=3, [0 ... 9]=3},
/*   4 */	{[115]=15, [105]=14},
/*   5 */	{},
/*   6 */	{[62]=16},
/*   7 */	{[42]=17},
/*   8 */	{},
/*   9 */	{[97 ... 122]=18, [95]=18, [65 ... 90]=18},
/*  10 */	{},
/*  11 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19},
/*  12 */	{[97 ... 122]=12, [95]=12, [65 ... 90]=12, [48 ... 57]=12, [39]=20},
/*  13 */	{},
/*  14 */	{[103]=21},
/*  15 */	{[107]=22},
/*  16 */	{},
/*  17 */	{[43 ... 255]=17, [42]=23, [0 ... 41]=17},
/*  18 */	{[97 ... 122]=18, [95]=18, [65 ... 90]=18, [62]=25, [48 ... 57]=18, [39]=24},
/*  19 */	{[97 ... 122]=19, [95]=19, [65 ... 90]=19, [48 ... 57]=19},
/*  20 */	{[39]=20},
/*  21 */	{[99]=26},
/*  22 */	{[105]=27},
/*  23 */	{[48 ... 255]=17, [47]=28, [43 ... 46]=17, [42]=23, [0 ... 41]=17},
/*  24 */	{[62]=25, [39]=24},
/*  25 */	{},
/*  26 */	{[97]=29},
/*  27 */	{[112]=30},
/*  28 */	{[43 ... 255]=17, [42]=23, [0 ... 41]=17},
/*  29 */	{[115]=31},
/*  30 */	{},
/*  31 */	{[101]=32},
/*  32 */	{},
};

static final_state_t fgfx_final_table[][2] = {
	{ 2, 	TSPACE },
	{ 3, 	TCOM },
	{ 5, 	TCOMMA },
	{ 8, 	TSEMI },
	{ 10, 	TEQUAL },
	{ 12, 	TG_IDENT },
	{ 13, 	TUNION },
	{ 16, 	TARROW },
	{ 19, 	TL_IDENT },
	{ 20, 	TNON_TER },
	{ 25, 	TENON_TER },
	{ 28, 	TCOM },
	{ 30, 	TSKIP },
	{ 32, 	TIGCASE },
};

#define SIZE_FINAL_TAB	14

static int fgfx_skip_table[] = {
	TSPACE,
	TCOM,
	-1,
};

#endif /* ONLY_TOKEN */
#endif /* FGFX_LEX_H */