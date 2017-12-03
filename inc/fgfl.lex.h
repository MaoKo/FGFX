#ifndef FGFL_LEX_H
#define FGFL_LEX_H

#include <stdint.h>

enum {
	TNONE,
	TG_IDENT,
	TL_IDENT,
	TEQUAL,
	TCOM,
	TSPACE,
	TSEMI,
	TIGCASE,
	TCOMMA,
	TEOF,
};

#define TOTAL_TOKEN	10

#ifndef ONLY_TOKEN

#define START_STATE	1
#define DEAD_STATE	0

//Size of state table = 17
static uint8_t fgfl_state_table[][256] = {
/*   0 */	{},
/*   1 */	{[97 ... 122]=9, [95]=9, [65 ... 90]=9, [64]=8, [61]=7, [59]=6, [44]=5, [37]=4, [35]=3, [32]=2, [9 ... 10]=2},
/*   2 */	{[32]=2, [9 ... 10]=2},
/*   3 */	{[11 ... 255]=3, [0 ... 9]=3},
/*   4 */	{[105]=10},
/*   5 */	{},
/*   6 */	{},
/*   7 */	{},
/*   8 */	{[97 ... 122]=11, [95]=11, [65 ... 90]=11},
/*   9 */	{[97 ... 122]=9, [95]=9, [65 ... 90]=9, [48 ... 57]=9},
/*  10 */	{[103]=12},
/*  11 */	{[97 ... 122]=11, [95]=11, [65 ... 90]=11, [48 ... 57]=11},
/*  12 */	{[99]=13},
/*  13 */	{[97]=14},
/*  14 */	{[115]=15},
/*  15 */	{[101]=16},
/*  16 */	{},
};

static uint8_t fgfl_final_table[][2] = {
	{ 2, 	TSPACE },
	{ 3, 	TCOM },
	{ 5, 	TCOMMA },
	{ 6, 	TSEMI },
	{ 7, 	TEQUAL },
	{ 9, 	TG_IDENT },
	{ 11, 	TL_IDENT },
	{ 16, 	TIGCASE },
};

#define SIZE_FINAL_TAB	8

#endif /* ONLY_TOKEN */
#endif /* FGFL_LEX_H */