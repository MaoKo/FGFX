#ifndef BITSET_H
#define BITSET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t		_SETTYPE;
#define	_BITS_IN_WORD		(sizeof(_SETTYPE) << 3)
#define _BYTE_SETTYPE(x)	(x * sizeof(_SETTYPE))
#define _DIV_WSIZE(x)		(x >> 6)
#define _MOD_WSIZE(x)		(x & 0x3F)
#define _DEFWORDS		16
#define _DEFBITS		(_DEFWORDS * _BITS_IN_WORD)
#define _ROUND(bit)		((_DIV_WSIZE(bit) << 3) << 1)

//Iterator Bitset
#define IT_NEXT(bs)		(_next_bitset(bs))
#define IT_RESET(bs)		((bs) ? bs->siter = 0 : 0)

#define OP_BITSET(bs, x, op)	(bs->map[_DIV_WSIZE(x)] op (1UL<<_MOD_WSIZE(x)))
#define IS_PRESENT(bs, x)	((x >= bs->nbits) ? false : (OP_BITSET(bs, x, &)))

#define ADD_BITSET(bs, x)	((x >= bs->nbits)  ? _add_bitset(bs, x)\
					: (OP_BITSET(bs, x, |=)))

#define OFF_BITSET(bs, x)	((x >= bs->nbits) ? 0 : (OP_BISET(bs, x, |= ~)))
#define CLEAR_BITSET(bs)	(memset(bs->map, 0, _BYTE_SETTYPE(bs->nwords)))

//Common operation available with set

enum {
	_UNION,
	_INTERSECT,
	_DIFF,
	_COMPL,
};

#define UNION_BITSET(x, y)	(_op_bitset(_UNION, x, y))
#define INTERSECT_BITSET(x, y)	(_op_bitset(_INTERSECT, x, y))
#define DIFF_BITSET(x, y)	(_op_bitset(_DIFF, x, y))
#define COMPL_BITSET(x)		(_op_bitset(_COMPL, x, NULL))

typedef struct {
	unsigned char nwords;
	unsigned int nbits;
	_SETTYPE* map;
	_SETTYPE defmap[_DEFWORDS];
	unsigned int siter;	
} bitset_t;

bitset_t* new_bitset(void);
void del_bitset(bitset_t*);
bitset_t* dup_bitset(bitset_t const*);
_SETTYPE _add_bitset(bitset_t*, size_t);
bool is_empty_bitset(bitset_t const*);
bool eq_bitset(bitset_t const*, bitset_t const*);
bitset_t* _op_bitset(int, bitset_t*, bitset_t*);
void truncate_bitset(bitset_t*);
int _next_bitset(bitset_t*);
_SETTYPE hash_bitset(bitset_t const*);
void print_bitset(bitset_t*);

#endif /* BITSET_H */
