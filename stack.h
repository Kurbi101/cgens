#ifndef _STACK_H
#define _STACK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void(destructor)(void *);

#define IS_POINTER_TYPE(T)                                                     \
	(__builtin_types_compatible_p(T, void *) ||                                \
	 __builtin_classify_type((T)0) == 5)

#define DEFINE_STACK(_name, _type)                                             \
                                                                               \
	typedef struct _name _name;                                                \
	typedef struct _##_name##_node _##_name##_node;                            \
	typedef struct _name##_iter _name##_iter;                                  \
                                                                               \
	struct _##_name##_node {                                                   \
		_type data;                                                            \
		_##_name##_node *next;                                                 \
	};                                                                         \
                                                                               \
	struct _name {                                                             \
		size_t len;                                                            \
		_##_name##_node *head;                                                 \
	};                                                                         \
                                                                               \
	struct _name##_iter {                                                      \
		bool has_next;                                                         \
		_##_name##_node *curr;                                                 \
	};                                                                         \
                                                                               \
	void _name##_push(_name *s, _type data) {                                  \
		if (!s->head) {                                                        \
			_##_name##_node *head =                                            \
				(_##_name##_node *)malloc(sizeof(_##_name##_node));            \
			if (!head) {                                                       \
				fprintf(stderr, "<<<ERROR: malloc failed inside stack push "   \
								"function >>>\n");                             \
				abort();                                                       \
			}                                                                  \
			head->data = data;                                                 \
			s->head = head;                                                    \
			s->len++;                                                          \
			return;                                                            \
		}                                                                      \
		_##_name##_node *new_node =                                            \
			(_##_name##_node *)malloc(sizeof(_##_name##_node));                \
		if (!new_node) {                                                       \
			fprintf(                                                           \
				stderr,                                                        \
				"<<<ERROR: malloc failed inside _name push function >>>\n");   \
			abort();                                                           \
		}                                                                      \
		new_node->data = data;                                                 \
		new_node->next = s->head;                                              \
		s->len++;                                                              \
		s->head = new_node;                                                    \
	}                                                                          \
                                                                               \
	_type _name##_peek(_name *s) {                                             \
		if (!s->head) {                                                        \
			fprintf(stderr,                                                    \
					"<<<ERROR: Attempting to peek at an empty stack >>>\n");   \
			abort();                                                           \
		}                                                                      \
		return s->head->data;                                                  \
	}                                                                          \
                                                                               \
	_type _name##_pop(_name *s) {                                              \
		if (!s->head) {                                                        \
			fprintf(stderr,                                                    \
					"<<<ERROR: Attempting to pop an empty stack >>>\n");       \
			abort();                                                           \
		}                                                                      \
                                                                               \
		_##_name##_node *prev_head = s->head;                                  \
		s->head = prev_head->next;                                             \
		_type result = prev_head->data;                                        \
		free(prev_head);                                                       \
		s->len--;                                                              \
		return result;                                                         \
	}                                                                          \
                                                                               \
	bool _name##_is_empty(_name *s) { return s->len == 0; }                    \
                                                                               \
	void _name##_clear(_name *s, destructor d) {                               \
		while (!_name##_is_empty(s)) {                                         \
			_type _temp = _name##_pop(s);                                      \
			if (d) {                                                           \
				d(_temp);                                                      \
			}                                                                  \
		}                                                                      \
	}                                                                          \
                                                                               \
	_name##_iter *_name##_to_iter(_name *s) {                                  \
		_name##_iter *it = (_name##_iter *)malloc(sizeof(_name##_iter));       \
		it->curr = s->head;                                                    \
		if (!it->curr) {                                                       \
			it->has_next = false;                                              \
			return it;                                                         \
		}                                                                      \
		it->has_next = true;                                                   \
		return it;                                                             \
	}                                                                          \
                                                                               \
	_type _name##_get_next(_name##_iter *it) {                                 \
		if (!it->has_next) {                                                   \
			fprintf(stderr,                                                    \
					"<<< ERROR: Stack iterator attempts to acces element "     \
					"that doesnt exsist!\n >>>\n");                            \
			abort();                                                           \
		}                                                                      \
		_type curr = it->curr->data;                                           \
		it->curr = it->curr->next;                                             \
		if (!it->curr)                                                         \
			it->has_next = false;                                              \
		return curr;                                                           \
	}
#endif