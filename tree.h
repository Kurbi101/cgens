/*
	generic tree implementation for C
*/

#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <string.h>

#define DEFINE_TREE(_node_name, _type, _cmp)                                   \
	typedef struct _node_name _node_name;                                      \
                                                                               \
	struct _node_name {                                                        \
		_type data;                                                            \
		_node_name *left;                                                      \
		_node_name *right;                                                     \
		size_t height;                                                         \
	};                                                                         \
                                                                               \
	_node_name *init_##_node_name(_type data) {                                \
		_node_name *__node = (_node_name *)malloc(sizeof(_node_name));         \
		__node->left = __node->right = NULL;                                   \
		__node->data = data;                                                   \
		__node->height = 0;                                                    \
		return __node;                                                         \
	}                                                                          \
                                                                               \
	static inline size_t __##_node_name##_get_height(_node_name *__node) {     \
		if (!__node)                                                           \
			return 0;                                                          \
		return __node->height;                                                 \
	}                                                                          \
                                                                               \
	size_t get_balance(_node_name *__node) {                                   \
		if (!__node)                                                           \
			return 0;                                                          \
		return __##_node_name##_get_height(__node->left) -                     \
			   __##_node_name##_get_height(__node->right);                     \
	}                                                                          \
                                                                               \
	static inline size_t __##_node_name##_max(size_t x, size_t y) {            \
		return x > y ? x : y;                                                  \
	}                                                                          \
                                                                               \
	_node_name *__##_node_name##_right_rotate(_node_name *y) {                 \
		_node_name *x = y->left;                                               \
		_node_name *T2 = x->right;                                             \
		x->right = y;                                                          \
		y->left = T2;                                                          \
		y->height =                                                            \
			__##_node_name##_max(__##_node_name##_get_height(y->left),         \
								 __##_node_name##_get_height(y->right)) +      \
			1;                                                                 \
		x->height =                                                            \
			__##_node_name##_max(__##_node_name##_get_height(x->left),         \
								 __##_node_name##_get_height(x->right)) +      \
			1;                                                                 \
		return x;                                                              \
	}                                                                          \
                                                                               \
	_node_name *__##_node_name##_left_rotate(_node_name *x) {                  \
		_node_name *y = x->right;                                              \
		_node_name *T2 = y->left;                                              \
		y->left = x;                                                           \
		x->right = T2;                                                         \
		x->height =                                                            \
			__##_node_name##_max(__##_node_name##_get_height(x->left),         \
								 __##_node_name##_get_height(x->right)) +      \
			1;                                                                 \
		y->height =                                                            \
			__##_node_name##_max(__##_node_name##_get_height(y->left),         \
								 __##_node_name##_get_height(y->right)) +      \
			1;                                                                 \
		return y;                                                              \
	}                                                                          \
                                                                               \
	_node_name *insert(_node_name *node, _type key) {                          \
		if (!node) {                                                           \
			node = init_node(key);                                             \
			return node;                                                       \
		}                                                                      \
		int compare_val = _cmp(key, node->data);                               \
		if (_cmp(key, node->data) < 0) {                                       \
			node->left = insert(node->left, key);                              \
		} else if (_cmp(key, node->data) > 0) {                                \
			node->right = insert(node->right, key);                            \
		} else {                                                               \
			return node;                                                       \
		}                                                                      \
		node->height =                                                         \
			__##_node_name##_max(__##_node_name##_get_height(node->left),      \
								 __##_node_name##_get_height(node->right)) +   \
			1;                                                                 \
		int balance = get_balance(node);                                       \
		if (balance > 1 && _cmp(key, node->left->data) < 0)                    \
			return __##_node_name##_right_rotate(node);                        \
		if (balance < -1 && _cmp(key, node->right->data) > 0)                  \
			return __##_node_name##_left_rotate(node);                         \
		if (balance > 1 && _cmp(key, node->left->data) > 0) {                  \
			node->left = __##_node_name##_left_rotate(node->left);             \
			return __##_node_name##_right_rotate(node);                        \
		}                                                                      \
		if (balance < -1 && _cmp(key, node->right->data) < 0) {                \
			node->right = __##_node_name##_right_rotate(node->right);          \
			return __##_node_name##_left_rotate(node);                         \
		}                                                                      \
		return node;                                                           \
	}
#endif