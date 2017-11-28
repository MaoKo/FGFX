#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>

#define BYTE_SZ	(sizeof(byte) * 8)
#define ROW(n)	(n / (int)BYTE_SZ)
#define COL(n)	(1 << (n % (int)BYTE_SZ))

#define MIN_BITSET	16
#define MAX_BITSET	64

//Iterator Bitset Move
#define NEXT		1
#define RESET		0

typedef uint8_t byte;
typedef uint16_t word;
typedef struct {
	word nword;
	word nbyte;
	byte* bmap;
	word iter_pos;
} bitset_t;

bitset_t* bitset(void);
void del_bitset(bitset_t*);
bitset_t* dup_bitset(bitset_t const*);
int add_bitset(bitset_t*, word);
bool has_item_bitset(bitset_t const*, word);
bool is_empty_bitset(bitset_t const*);
bool cmp_bitset(bitset_t const*, bitset_t const*);
void union_bitset(bitset_t*, bitset_t const*);
void intersect_bitset(bitset_t*, bitset_t const*);
void compl_bitset(bitset_t*);
int iter_bitset(bitset_t*, int);
void print_bitset(bitset_t const*);

#endif /* BITSET_H */
