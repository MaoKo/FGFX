#ifndef FGFL_LEX_H
#define FGFL_LEX_H

#include <stdint.h>
typedef	uint8_t	dfa_state_t;
typedef	uint8_t	final_state_t;

enum {
	TNONE,
	TG_IDENT,
	TL_IDENT,
	TEQUAL,
	TCOM,
	TSPACE,
	TSEMI,
	TIGCASE,
	TSKIP,
	TCOMMA,
	TEOF,
};

#define TOTAL_TOKEN	11

#ifndef ONLY_TOKEN

#define START_STATE	1
#define DEAD_STATE	0

//Size of state table = 21
static dfa_state_t fgfl_state_table[][256] = {
/*   0 */	{},
/*   1 */	{[97 ... 122]=9, [95]=9, [65 ... 90]=9, [64]=8, [61]=7, [59]=6, [44]=5, [37]=4, [35]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[11 ... 255]=3, [0 ... 9]=3},
/*   4 */	{[115]=11, [105]=10},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{[97 ... 122]=12, [95]=12, [65 ... 90]=12},
/*   9 */	{[97 ... 122]=9, [95]=9, [65 ... 90]=9, [48 ... 57]=9},
/*  10 */	{[103]=13},
/*  11 */	{[107]=14},
/*  12 */	{[97 ... 122]=12, [95]=12, [65 ... 90]=12, [48 ... 57]=12},
/*  13 */	{[99]=15},
/*  14 */	{[105]=16},
/*  15 */	{[97]=17},
/*  16 */	{[112]=18},
/*  17 */	{[115]=19},
/*  18 */	{},
/*  19 */	{[101]=20},
/*  20 */	{},
};

static final_state_t fgfl_final_table[][2] = {
	{ 2, 	TSPACE },
	{ 3, 	TCOM },
	{ 5, 	TCOMMA },
	{ 6, 	TSEMI },
	{ 7, 	TEQUAL },
	{ 9, 	TG_IDENT },
	{ 12, 	TL_IDENT },
	{ 18, 	TSKIP },
	{ 20, 	TIGCASE },
};

#define SIZE_FINAL_TAB	9

static int fgfl_skip_table[] = {
	TCOM,
	TSPACE,
	-1,
};

#endif /* ONLY_TOKEN */
#endif /* FGFL_LEX_H */