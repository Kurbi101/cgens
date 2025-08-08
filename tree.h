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
		const _type data;                                                      \
		_node_name *left;                                                      \
		_node_name *right;                                                     \
		size_t height;                                                         \
	};                                                                         \
                                                                               \
	_node_name *init_node(const _type data) {                                  \
		_node_name *node = (_node_name *)malloc(sizeof(_node_name));           \
		node->left = node->right = NULL;                                       \
		node->height = 0;                                                      \
		return node;                                                           \
	}                                                                          \
                                                                               \
	static inline size_t get_height(_node_name *node) {                        \
		if (!node)                                                             \
			return 0;                                                          \
		return node->height;                                                   \
	}                                                                          \
                                                                               \
	size_t get_balance(_node_name *node) {                                     \
		if (!node)                                                             \
			return 0;                                                          \
		return get_height(node->left) - get_height(node->right);               \
	}                                                                          \
                                                                               \
	static inline size_t max(size_t x, size_t y) { return x > y ? x : y; }     \
                                                                               \
	_node_name *right_rotation(_node_name *y) {                                \
		_node_name *x = y->left;                                               \
		_node_name *T2 = x->right;                                             \
		x->right = y;                                                          \
		y->left = T2;                                                          \
		y->height = max(get_height(y->left), get_height(y->right)) + 1;        \
		x->height = max(get_height(x->left), get_height(x->right)) + 1;        \
		return x;                                                              \
	}                                                                          \
                                                                               \
	_node_name *left_rotation(_node_name *x) {                                 \
		_node_name *y = x->right;                                              \
		_node_name *T2 = y->left;                                              \
		y->left = x;                                                           \
		x->right = T2;                                                         \
		x->height = max(get_height(x->left), get_height(x->right)) + 1;        \
		y->height = max(get_height(y->left), get_height(y->right)) + 1;        \
		return y;                                                              \
	}                                                                          \
                                                                               \
	_node_name *insert(_node_name *node, const _type key) {                    \
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
			max(get_height(node->left), get_height(node->right)) + 1;          \
		int balance = get_balance(node);                                       \
		if (balance > 1 && _cmp(key, node->left->data) < 0)                    \
			return right_rotation(node);                                       \
		if (balance < -1 && _cmp(key, node->right->data) > 0)                  \
			return left_rotation(node);                                        \
		if (balance > 1 && _cmp(key, node->left->data) > 0) {                  \
			node->left = left_rotation(node->left);                            \
			return right_rotation(node);                                       \
		}                                                                      \
		if (balance < -1 && _cmp(key, node->right->data) < 0) {                \
			node->right = right_rotation(node->right);                         \
			return left_rotation(node);                                        \
		}                                                                      \
		return node;                                                           \
	}
    
#endif