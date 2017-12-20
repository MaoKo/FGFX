#ifndef _TEST_GRM_H
#define _TEST_GRM_H

#define ONLY_TOKEN	
#include "none.h"
#undef	ONLY_TOKEN

enum {
	N_A,
};

#define TOTAL_NTER	1

//Various macro representing production.
#define P_NA_Ta	1


#define ERROR_SLOT	0
#define START_SYMBOL	N_A

static int
test_ll_table[TOTAL_NTER][TOTAL_TOKEN] = {
	[N_A][Ta]=P_NA_Ta, 
};

#if 0
enum{
	_SHIFT	= 0x1000,
	_REDUCE	= 0x2000,
	_GOTO	= 0x4000,
};

#define SHIFT(x)	(x | _SHIFT)
#define REDUCE(x)	(x | _REDUCE)
#define GOTO(x)	(x | _GOTO)
#define ACCEPT	(~0)

#define LR_START_STATE	0

static int
lr_action_table[1][TOTAL_TOKEN] = {
/*   0 */	{},
};

static int
lr_goto_table[1][TOTAL_TOKEN] = {
/*   0 */	{},
};

#endif
static int
test_sync_token[TOTAL_NTER][2] = {
	[N_A] = { T_EOF, -1 },
};

#endif /* _TEST_GRM_H */
