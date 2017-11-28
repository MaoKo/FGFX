#include "utils.h"
#include "bitset.h"

static int
alloc_bitmap(bitset_t* bits, word old) {
	if (bits->bmap) {
		byte* chunk = REALLOC(bits->bmap, bits->nword);
		if (!chunk)
			{ return (-1); }	
		bits->bmap = chunk;
	}
	else {
		bits->bmap = NEW(byte, bits->nword);
		if (!bits->bmap)
			{ return (-1); }
	}
	bits->nbyte = bits->nword * BYTE_SZ;
	memset(bits->bmap + old, 0, (bits->nword - old));
	return (0);
}

static bitset_t*
make_bitset(int size) {
	bitset_t* bits = NEW(bitset_t, 1);
	if (!bits)
		{ return (NULL); }
	bits->nword = size;
	bits->bmap = NULL;
	bits->iter_pos = 0;
	if (alloc_bitmap(bits, 0)) {
		FREE(bits);
		return (NULL);
	}
	return (bits);
}

bitset_t*
bitset(void) {
	return (make_bitset(MIN_BITSET));
}

void
del_bitset(bitset_t* bits) {
	if (bits)
		{ FREE(bits->bmap); }
	FREE(bits);
}

bitset_t*
dup_bitset(bitset_t const* bits) {
	if (!bits)
		{ return (NULL); }
	bitset_t* new_bitmap = make_bitset(bits->nword);
	memcpy(new_bitmap->bmap, bits->bmap, bits->nword);
	return (new_bitmap);
}

int
add_bitset(bitset_t* bits, word nb) {
	if (!bits)
		{ return (-1); }
	if (nb >= bits->nbyte) {
		if (ROW(nb) >= MAX_BITSET)
			{ return (-1); }
		word old = bits->nword;
		bits->nword = ROW(nb) + 1;
		if (alloc_bitmap(bits, old))
			{ return (-1); }
	}
	bits->bmap[ROW(nb)] |= COL(nb);	
	return (0);
}

bool
has_item_bitset(bitset_t const* bits, word nb) {
	if (!bits)
		{ return (false); }
	return (bits->bmap[ROW(nb)] & COL(nb));	
}

bool
is_empty_bitset(bitset_t const* bits) {
	if (!bits)
		{ return (false); }
	for (size_t i = 0; i < bits->nword; ++i) {
		if (bits->bmap[i] != 0)
			{ return (false); }
	}
	return (true);
}

bool
cmp_bitset(bitset_t const* b1, bitset_t const* b2) {
	if (!b1 || !b2)
		{ return (!b1 && !b2); }
	if (b1->nword != b2->nword)
		{ return (false); }
	for (size_t i = 0; i < b1->nword; ++i) {
		if (b1->bmap[i] != b2->bmap[i])
			{ return (false); }
	}
	return (true);
}

void
union_bitset(bitset_t* bits, bitset_t const* lbits) {
	if (!bits || !lbits)
		{ return; }
	word max = MIN(bits->nword, lbits->nword);
	for (int i = 0; i < max; ++i)
		{ bits->bmap[i] |= lbits->bmap[i]; }
}

void
intersect_bitset(bitset_t* bits, bitset_t const* lbits) {
	if (!bits || !lbits)
		{ return; }
	word max = MIN(bits->nword, lbits->nword);
	for (int i = 0; i < max; ++i)
		{ bits->bmap[i] &= lbits->bmap[i]; }
}

void
compl_bitset(bitset_t* bits) {
	for (size_t i = 0; i < bits->nword; ++i)
		{ bits->bmap[i] = ~bits->bmap[i]; }
}

int
iter_bitset(bitset_t* bits, int action) {
	if (!bits)
		{ return (-1); }
	if (action == RESET)
		{ bits->iter_pos = 0; }
	else if (action == NEXT) {
		while (bits->iter_pos < bits->nbyte) {
			if (bits->bmap[ROW(bits->iter_pos)] & COL(bits->iter_pos))
				{ return (bits->iter_pos++); }
			++bits->iter_pos;
		}
	}
	return (-1);
}

void
print_bitset(bitset_t const* bits) {
	if (!bits)
		{ return; }
	printf("{");
	for (size_t i = 0; i < bits->nword; ++i) {
		for (size_t j = 0; j < BYTE_SZ; ++j) {
			if (bits->bmap[i] & COL(j))
				{ printf("%zu,", j + (i * BYTE_SZ)); }
		}
	}
	printf("}\n");
}

