#include "set.h"

set_t* set(void* item) {
	set_t* st = NEW(set_t, 1);
	if (!st)
		{ return (EMPTY_SET); }
	st->item = item; st->next = EMPTY_SET;
	return (st);
}

void del_set(set_t* st) {
	if (st) {
		del_set(st->next);
		FREE(st);
	}
}

set_t* cpy_set(set_t const* st) {
	if (st) {
		set_t* next = cpy_set(st->next);
		set_t* node = set(st->item);
		if (!node)
			{ return (EMPTY_SET); }
		node->next = next;
		return (node);
	}
	return (EMPTY_SET);
}

bool has_set(set_t const* st, void* item) {
	if (!st)
		{ return (false); }
	set_t const* it = st;
	while (it) {
		if (it->item == item)
			{ return (true); }
		it = it->next;
	}
	return (false);
}

void add_set(set_t** st, void* item) {
	if (!st)
		{ return; }
	if (!has_set(*st, item)) {
		set_t* new_st = set(item);
		if (!new_st)
			{ return; }
		new_st->next = (*st);
		(*st) = new_st;
	}
}

void union_set(set_t** ssrc, set_t const* sdst) {
	set_t const* it = sdst;
	while (it) {
		add_set(ssrc, it->item);
		it = it->next;
	}
}

set_t* intersection_set(set_t const* s1, set_t const* s2) {
	set_t* inter = EMPTY_SET;
	set_t const* it = s2;
	while (it) {
		if (has_set(s1, it->item))
			{ add_set(&inter, it->item); }
		it = it->next;
	}
	return (inter);
}

static bool is_subset(set_t const* s1, set_t const* s2) {
	set_t const* it = s2;
	while (it) {
		if (!has_set(s1, it->item))
			{ return (false); }
		it = it->next;
	}
	return (true);
}

bool equal_set(set_t const* s1, set_t const* s2) {
	return (is_subset(s1, s2) && is_subset(s2, s1));
}

void print_set(set_t const* st) {
	printf("{");
	set_t const* it = st;
	while (it) {
		printf("%p", it->item);
		if (it->next)
			{ printf(", "); }
		it = it->next;
	}
	printf("}\n");
}
