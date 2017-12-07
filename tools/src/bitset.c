#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "bitset.h"
#include "vector.h"
#include "utils.h"

static int
adjust_bitset(bitset_t* bs, unsigned char new_size) {
	if (!bs || new_size <= bs->nwords)
		{ return (0); }
	int tail	= new_size - bs->nwords;
	bs->nwords	= new_size;
	bs->nbits	= new_size * _BITS_IN_WORD;
	_SETTYPE* chunk	= NEW(_SETTYPE, _BYTE_SETTYPE(new_size));
	if (!chunk)
		{ return (-1); }
	if (bs->map == bs->defmap) { 
		bs->map	= chunk;
		memcpy(bs->map, bs->defmap, _BYTE_SETTYPE(_DEFWORDS));
	}
	else {
		memcpy(chunk, bs->map, _BYTE_SETTYPE(_DEFWORDS));
		bs->map	= chunk;
	}
	memset(bs->map + tail, 0, _BYTE_SETTYPE(bs->nwords) - tail);
	return (0);
}

bitset_t*
new_bitset(void) {
	bitset_t* bs	= NEW(bitset_t, 1);
	if (!bs)
		{ return (NULL); }
	memset(bs, 0, sizeof(bitset_t));
	bs->nwords	= _DEFWORDS;
	bs->nbits	= _DEFBITS;
	bs->map		= bs->defmap;
	return (bs);
}

void
del_bitset(bitset_t* bs) {
	if (bs && bs->map != bs->defmap)
		{ FREE(bs->map); }
	FREE(bs);
}

bitset_t*
dup_bitset(bitset_t const* bs) {
	if (!bs)
		{ return (NULL); }
	bitset_t* nbs		= new_bitset();
	memcpy(nbs, bs, sizeof(bitset_t));
	if (bs->map == bs->defmap)
		{ nbs->map	= nbs->defmap; }
	else {
		size_t bs_size	= nbs->nwords * sizeof(_SETTYPE);
		if (!(nbs->map	= NEW(_SETTYPE, bs_size)))
			{ return (NULL); }
		memcpy(nbs->map, bs->map, bs_size);
	}
	return (nbs);
}

_SETTYPE
_add_bitset(bitset_t* bs, size_t n) {
	if (!bs || n < bs->nbits)
		{ return (0); }
	if (adjust_bitset(bs, _ROUND(n)))
		{ return (-1); }
	return (ADD_BITSET(bs, n));
}

bool
is_empty_bitset(bitset_t const* bs) {
	if (!bs)
		{ return (true); }
	for (size_t i = 0; i < bs->nwords; ++i) {
		if (bs->map[i])
			{ return (false); }
	}
	return (true);
}

bool
eq_bitset(bitset_t const* b1, bitset_t const* b2) {
	if (!b1 || !b2)
		{ return (!b1 && !b2); }
	for (size_t i = 0; (i < b1->nwords) && (i < b2->nwords); ++i) {
		if (b1->map[i] != b2->map[i])
			{ return (false); }
	}
	if (b1->nwords != b2->nwords) {
		bitset_t const* max_bset = ((b1->nwords > b2->nwords) ? b1 : b2);
		int leading = MAX(b1->nwords, b2->nwords);
		for (size_t i = leading; i < max_bset->nwords; ++i) {
			if (max_bset->map[i] != 0)
				{ return (false); }
		}
	}
	return (true);
}

bitset_t*
_op_bitset(int kind, bitset_t* rbs, bitset_t* lbs) {
	if (!rbs || (kind != _COMPL && !lbs))
		{ return (NULL); }
	if (kind != _COMPL && (adjust_bitset(rbs, lbs->nwords)
			|| adjust_bitset(lbs, rbs->nwords)))
		{ return (NULL); }
	for (size_t i = 0; i < rbs->nwords; ++i) {
		switch (kind) {
			case _UNION:	rbs->map[i] |= lbs->map[i];
					break;
			case _INTERSECT:
					rbs->map[i] &= lbs->map[i];
					break;
			case _DIFF:	rbs->map[i] = (rbs->map[i]
						^ (rbs->map[i] & lbs->map[i]));
					break;
			case _COMPL:	rbs->map[i] = ~rbs->map[i];
					break;
		}
	}
	return (rbs);
}

void
truncate_bitset(bitset_t* bs) {
	if (!bs)
		{ return; }
	if (bs->map != bs->defmap) {
		FREE(bs->map);
		bs->map		= bs->defmap;
		bs->nwords	= _DEFWORDS;
		bs->nbits	= _DEFBITS;
	}
	CLEAR_BITSET(bs);
}

int
_next_bitset(bitset_t* bs) {
	if (!bs)
		{ return (-1); }
	while (bs->siter < bs->nbits) {
		if (bs->map[_DIV_WSIZE(bs->siter)] & (1UL<<_MOD_WSIZE(bs->siter)))
			{ return (bs->siter++); }
		++bs->siter;
	}
	return (-1);
}

_SETTYPE
hash_bitset(bitset_t const* bs) {
	if (!bs)
		{ return (0); }
	_SETTYPE count = 0;
	//Don't matter if overflow occurs
	for (size_t i = 0; i < bs->nwords; ++i)
		{ count += bs->map[i]; }
	return (count);
}

void
print_bitset(bitset_t* bs) {
	printf("{");
	if (bs) {
		unsigned int iter_back = bs->siter;
		int i;
		while ((i = IT_NEXT(bs)) != -1)
			{ printf("%d,", i); }
		bs->siter = iter_back;
	}
	printf("}\n");
}

bool
is_subset_bitset(bitset_t const* bset1, bitset_t const* bset2) {
	if (!bset1 || !bset2)
		{ return (!bset2); }
	//The Empty set is the subset of every all set even the empty set
	unsigned char max = MAX(bset1->nwords, bset2->nwords);
	for (size_t i = 0; i < max; ++i) {
		if ((~bset1->map[i]) & bset2->map[i])
			{ return (false); }
	}
	if (bset2->nwords > bset1->nwords) {
		for (size_t i = bset1->nwords; i < bset2->nwords; ++i) {
			if (bset2->map[i])
				{ return (false); }
		}
	}
	return (true);
}

bool
is_disjoint_bitset(bitset_t const* bset1, bitset_t const* bset2) {
	if (!bset1 || !bset2)
		{ return (!(!bset1 && !bset2)); }
	unsigned char max = MAX(bset1->nwords, bset2->nwords);
	for (size_t i = 0; i < max; ++i) {
		if (bset1->map[i] & bset2->map[i])
			{ return (false); }
	}
	return (true);
}
