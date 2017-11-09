#include "vector.h"

static vector_t* make_len_vector(size_t len) {
	vector_t* vect = NEW(vector_t, 1);
	if (!vect)
		{ return (NULL); }
	vect->body = NEW(void*, len);
	if (!vect->body)
		{ return (NULL); }
	vect->alloc = len;
	vect->index = 0;
	return (vect);
}

vector_t* vector(void) {
	return (make_len_vector(VECT_SIZE));
}

vector_t* cpy_vector(vector_t const* vect) {
	if (!vect)
		{ return (NULL); }
	vector_t* new_vect = make_len_vector(vect->alloc);
	if (!new_vect)
		{ return (NULL); }
	return (memcpy(new_vect->body, vect->body, sizeof(void*) * vect->alloc));
}

void del_vector(vector_t* vect) {
	if (vect) {
		FREE(vect->body);
		FREE(vect);
	}
}

void* at_vector(vector_t const* vect, size_t index) {
	if (!vect || vect->index <= index)
		{ return (NULL); }
	return (vect->body[index]);
}

void set_vector(vector_t* vect, size_t index, void* obj) {
	if (!vect || vect->index <= index)
		{ return; }
	vect->body[index] = obj;
}

void* front_vector(vector_t const* vect) {
	if (!vect || !vect->index)
		{ return (NULL); }
	return (*vect->body);
}

void* back_vector(vector_t const* vect) {
	if (!vect || !vect->index)
		{ return (NULL); }
	return (vect->body[vect->index - 1]);
}

bool empty_vector(vector_t const* vect) {
	if (!vect)
		{ return (true); }
	return (!vect->index);
}

size_t size_vector(vector_t const* vect) {
	if (!vect)
		{ return (0); }
	return (vect->index);
}

void clear_vector(vector_t* vect) {
	if (!vect)
		{ return; }
	bzero(vect->body, sizeof(void*) * vect->index);
	vect->index = 0;
}

static void extend_vector(vector_t* vect) {
	size_t new_sz;
	if (vect->index + 1 >= vect->alloc) {
		new_sz = round_up(vect->index + 1);
		void** new_body = REALLOC(vect->body, sizeof(void*) * new_sz);
		if (new_body) {
			vect->body = new_body;
			vect->alloc = new_sz;
		}
	}
}

void insert_vector(vector_t* vect, size_t offset, void* obj) {
	if (!vect)
		{ return; }
	extend_vector(vect);
	if (offset >= vect->index)
		{ vect->body[vect->index] = obj; }
	else {
		memmove(vect->body + offset + 1, vect->body + offset,
				sizeof(void*) * (vect->index - offset));
		vect->body[offset] = obj;
	}
	++vect->index;
}

void erase_vector(vector_t* vect, size_t offset) {
	if (!vect || vect->index <= offset)
		{ return; }
	if (offset + 1 != vect->index) {
		memmove(vect->body + offset, vect->body + offset + 1,
				sizeof(void*) * (vect->index - offset - 1));
	}
	--vect->index;
}

void push_back_vector(vector_t* vect, void* item) {
	if (!vect)
		{ return; }
	extend_vector(vect);
	vect->body[vect->index++] = item;
}

void* pop_back_vector(vector_t* vect) {
	if (!vect || !vect->index)
		{ return (NULL); }
	return (vect->body[--vect->index]);
}

void reverse_vector(vector_t* vect) {
	if (!vect)
		{ return; }
	size_t middle = vect->index / 2;
	for (size_t i = 0; i < middle; ++i) {
		void* swap_data = vect->body[i];
		vect->body[i] = vect->body[vect->index - i - 1];
		vect->body[vect->index - i - 1] = swap_data;
	}
}

