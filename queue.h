#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFINE_QUEUE(name, type)                                               \
	typedef struct queue queue;                                                \
	typedef struct name##_node name##_node;                                    \
	typedef struct name##_iter name##_iter;                                    \
	typedef void(name##destructor)(type);                                      \
                                                                               \
	struct queue {                                                             \
		size_t len;                                                            \
		name##_node *head;                                                     \
		name##_node *tail;                                                     \
	};                                                                         \
                                                                               \
	struct name##_node {                                                       \
		type data;                                                             \
		name##_node *next;                                                     \
	};                                                                         \
                                                                               \
	struct name##_iter {                                                       \
		bool has_next;                                                         \
		name##_node *curr;                                                     \
	};                                                                         \
                                                                               \
	void name##_push(queue *q, type data) {                                    \
		if (!q->head) {                                                        \
			q->head = malloc(sizeof(name##_node));                             \
			q->head->data = data;                                              \
			q->tail = q->head;                                                 \
			q->len++;                                                          \
			return;                                                            \
		}                                                                      \
		name##_node *_new = malloc(sizeof(name##_node));                       \
		_new->data = data;                                                     \
		q->tail->next = _new;                                                  \
		q->tail = _new;                                                        \
		q->len++;                                                              \
		return;                                                                \
	}                                                                          \
                                                                               \
	bool name##_is_empty(queue *q) { return q->len == 0; }                     \
                                                                               \
	type name##_peek(queue *q) {                                               \
		if (name##_is_empty(q)) {                                              \
			fprintf(                                                           \
				stderr,                                                        \
				"<<< ERROR: Attempting to peek at an empty Queue! >>> \n");    \
			abort();                                                           \
		}                                                                      \
		return q->head->data;                                                  \
	}                                                                          \
                                                                               \
	type name##_pop(queue *q) {                                                \
		if (name##_is_empty(q)) {                                              \
			fprintf(stderr,                                                    \
					"<<< ERROR: Attempting to pop an empty Queue! >>> \n");    \
			abort();                                                           \
		}                                                                      \
		name##_node *prev_head = q->head;                                      \
		q->head = q->head->next;                                               \
		type res = prev_head->data;                                            \
		free(prev_head);                                                       \
		q->len--;                                                              \
		return res;                                                            \
	}                                                                          \
                                                                               \
	void name##_clear(queue *q, name##destructor d) {                          \
		while (!name##_is_empty(q)) {                                          \
			type _temp = name##_pop(q);                                        \
			if (d) {                                                           \
				d(_temp);                                                      \
			}                                                                  \
		}                                                                      \
	}                                                                          \
                                                                               \
	void name##_free(queue *q, name##destructor d) {                           \
		name##_clear(q, d);                                                    \
		free(q);                                                               \
	}                                                                          \
                                                                               \
	name##_iter *name##_to_iter(queue *q) {                                    \
		if (!q)                                                                \
			return NULL;                                                       \
		name##_iter *it = malloc(sizeof(name##_iter));                         \
		it->has_next = q->head != NULL;                                        \
		it->curr = q->head;                                                    \
		return it;                                                             \
	}                                                                          \
                                                                               \
	bool name##_has_next(name##_iter *it) { return it->curr == NULL; }         \
                                                                               \
	type name##_get_next(name##_iter *it) {                                    \
		type res = it->curr->data;                                             \
		it->curr = it->curr->next;                                             \
		return res;                                                            \
	}

#endif