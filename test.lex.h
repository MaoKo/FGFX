#ifndef _TEST_LEX_H_
#define _TEST_LEX_H_

#include <stdint.h>
#include <stdbool.h>

enum {
	T_A,
	T_ERROR,
	T_EOF,
};

#define TOTAL_TOKEN	3

#ifndef _ONLY_STATE_TOKEN_

#define START_STATE	1
#define DEAD_STATE	0

static uint8_t
test_state_table[4][256] = {
/*   0 */	{},
/*   1 */	{[97]=2},
/*   2 */	{[98]=3},
/*   3 */	{},
};

static uint8_t
test_middle_table[4] = {
	[2]=true,
};

static uint8_t
test_final_table[2][2] = {
	{ 3, 	T_A },
	{ 0 },
};

static int8_t
test_look_table[2] = {
	T_A,
	-1,
};

#define SKIP_TABLE_NOT_DEFINE	

#endif /* _ONLY_STATE_TOKEN_ */
#endif /* _TEST_LEX_H_ */
