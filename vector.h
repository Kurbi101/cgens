#ifndef VECTOR_H
#define VECTOR_H

#include "error.h"
#include "types.h"
#include <stdlib.h>

#define VECTOR_CAP 6
#define VECTOR_GROWTH_FACTOR 2

#define DEFINE_VECTOR(name, type)                                              \
	typedef struct name name;                                                  \
	typedef struct name##_iter name##_iter;                                    \
                                                                               \
	typedef void name##_elem_destructor(type);                                 \
	typedef int name##_cmp(const type, const type);                            \
                                                                               \
	struct name {                                                              \
		type *data;                                                            \
		u64 len;                                                               \
		u64 cap;                                                               \
	};                                                                         \
                                                                               \
	struct name##_iter {                                                       \
		name *v;                                                               \
		u64 pos;                                                               \
		bool has_next;                                                         \
	};                                                                         \
                                                                               \
	void __##name##_resize_if_needed(name *v) {                                \
		if (v->len == v->cap - 1) {                                            \
			v->cap *= VECTOR_GROWTH_FACTOR;                                    \
			v->data = realloc(v->data, v->cap * sizeof(type));                 \
		}                                                                      \
	}                                                                          \
                                                                               \
	name *name##_init() {                                                      \
		name *v = malloc(sizeof(name));                                        \
		if (!v) {                                                              \
			ERROR("Malloc failed while initializing vector");                  \
		}                                                                      \
		v->cap = VECTOR_CAP;                                                   \
		v->data = calloc(1, sizeof(type) * v->cap);                            \
		if (!v->data) {                                                        \
			ERROR("Calloc failed while allocating vector data");               \
		}                                                                      \
		v->len = 0;                                                            \
		return v;                                                              \
	}                                                                          \
                                                                               \
	void name##_push(name *v, type val) {                                      \
		__##name##_resize_if_needed(v);                                        \
		v->data[v->len++] = val;                                               \
	}                                                                          \
                                                                               \
	void name##_set(name *v, u64 index, type val) {                            \
		if (index > v->len) {                                                  \
			ERROR("Index %llu out of bounds in name (len %llu)", index,        \
				  v->len);                                                     \
		}                                                                      \
		v->data[index] = val;                                                  \
		if (index == v->len)                                                   \
			v->len++;                                                          \
	}                                                                          \
                                                                               \
	type name##_get(name *v, u64 index) {                                      \
		if (index >= v->len) {                                                 \
			ERROR("Index %llu out of bounds in name (len %llu)", index,        \
				  v->len);                                                     \
		}                                                                      \
		return v->data[index];                                                 \
	}                                                                          \
                                                                               \
	bool name##_is_empty(name *v) { return v->len == 0; }                      \
                                                                               \
	void name##_clear(name *v, name##_elem_destructor d) {                     \
		if (d) {                                                               \
			for (size_t _i = 0; _i < v->len; ++_i) {                           \
				d(v->data[_i]);                                                \
			}                                                                  \
		}                                                                      \
		v->len = 0;                                                            \
	}                                                                          \
                                                                               \
	void name##_swap(name *v, i64 i1, i64 i2) {                                \
		if (i1 >= v->len || i2 >= v->len) {                                    \
			ERROR("Swapped indicies (%llu, %llu) out of bounds in "            \
				  "vector "                                                    \
				  "(len %llu)",                                                \
				  i1, i2, v->len);                                             \
		}                                                                      \
		type temp = v->data[i1];                                               \
		v->data[i1] = v->data[i2];                                             \
		v->data[i2] = temp;                                                    \
	}                                                                          \
                                                                               \
	u64 __##name##_partition(name *v, i64 low, i64 high, name##_cmp _cmp) {    \
                                                                               \
		type pivot = v->data[low];                                             \
		i64 i = low;                                                           \
		i64 j = high;                                                          \
                                                                               \
		while (i < j) {                                                        \
			while (_cmp(v->data[i], pivot) <= 0 && i <= high - 1)              \
				++i;                                                           \
			while (_cmp(v->data[j], pivot) > 0 && j >= low + 1)                \
				--j;                                                           \
                                                                               \
			if (i < j) {                                                       \
				name##_swap(v, i, j);                                          \
			}                                                                  \
		}                                                                      \
		name##_swap(v, low, j);                                                \
		return j;                                                              \
	}                                                                          \
                                                                               \
	void __##name##_quicksort(name *v, i64 low, i64 high, name##_cmp _cmp) {   \
		if (low < high) {                                                      \
                                                                               \
			u64 pivot = __##name##_partition(v, low, high, _cmp);              \
                                                                               \
			__##name##_quicksort(v, low, pivot - 1, _cmp);                     \
			__##name##_quicksort(v, pivot + 1, high, _cmp);                    \
		}                                                                      \
	}                                                                          \
                                                                               \
	void name##_sort(name *v, name##_cmp _cmp) {                               \
		__##name##_quicksort(v, 0, v->len - 1, _cmp);                          \
	}                                                                          \
                                                                               \
	name##_iter *name##_to_iter(name *v) {                                     \
		name##_iter *it = malloc(sizeof(name##_iter));                         \
		if (!it) {                                                             \
			ERROR("Malloc failed while initializing vector iterator");         \
		}                                                                      \
		it->v = v;                                                             \
		it->pos = 0;                                                           \
		it->has_next = v->len != 0;                                            \
		return it;                                                             \
	}                                                                          \
                                                                               \
	type name##_get_next(name##_iter *it) {                                    \
		if (it->pos >= it->v->len) {                                           \
			ERROR("Iterator position %llu out of bounds in vector (len %llu)", \
				  it->pos, it->v->len);                                        \
		}                                                                      \
		it->has_next = it->pos + 1 != it->v->len;                              \
		return name##_get(it->v, it->pos++);                                   \
	}

#endif