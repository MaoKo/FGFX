#include "fgfl.lex.h"
typedef unsigned short int grm_cell_t;

#define OFFSET		((sizeof(grm_cell_t) * 8) / sizeof(unsigned char))

enum {
	_SHIFT	=	(1 << OFFSET),
	_REDUCE	=	(2 << OFFSET),
	ACCEPT	=	(~0),
	ERROR	=	(0),
};

#define SHIFT(x)	(_SHIFT | x)
#define REDUCE(x)	(_REDUCE | x)
#define GOTO(x)		(x)

grm_cell_t action_table[][TOTAL_TOKEN] = {
	/*TG_IDENT	TL_IDENT	TEQUAL		TCOM	TSPACE	TSEMI		TEOF*/
/* S0 */{ SHIFT(3),	SHIFT(2),	ERROR,		ERROR,	ERROR,	ERROR,		ERROR, },
/* S1 */{ ERROR,	ERROR,		ERROR,		ERROR,	ERROR,	ERROR,		ACCEPT, },
/* S2 */{ ERROR,	ERROR,		REDUCE(2),	ERROR,	ERROR,	ERROR,		ERROR, },
/* S3 */{ ERROR,	ERROR,		REDUCE(3),	ERROR,	ERROR,	ERROR,		ERROR, },
/* S4 */{ ERROR,	ERROR,		SHIFT(5), 	ERROR,	ERROR, 	ERROR,		ERROR, },
/* S5 */{ ERROR,	ERROR,		ERROR,		ERROR,	ERROR,	SHIFT(6),	ERROR },
/* S6 */{ ERROR,	ERROR,		ERROR,		ERROR,	ERROR,	ERROR,		REDUCE(1) },
};

enum {
	NLineDef,
	NAtom
};

#define TOTAL_NON_TERMINAL 2

grm_cell_t goto_table[][TOTAL_NON_TERMINAL] = {
	/*NLineDef	NAtom*/
	{ GOTO(1),	GOTO(4), },
	{ ERROR,	ERROR, },
	{ ERROR,	ERROR, },
	{ ERROR,	ERROR, },
	{ ERROR,	ERROR, },
	{ ERROR,	ERROR, },
	{ ERROR,	ERROR, },
};
