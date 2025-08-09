/*
	generic tree implementation for C
*/

#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

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
		if (!__node) ERROR("Malloc failed while initializing a tree node");	   \
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
	size_t _name##_get_balance(_node_name *__node) {                           \
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
	_node_name *_node_name##_insert(_node_name *node, _type key) {             \
		if (!node) {                                                           \
			node = init_##_node_name(key);                                     \
			return node;                                                       \
		}                                                                      \
		if (_cmp(key, node->data) < 0) {                                       \
			node->left = _node_name##_insert(node->left, key);                 \
		} else if (_cmp(key, node->data) > 0) {                                \
			node->right = _node_name##_insert(node->right, key);               \
		} else {                                                               \
			return node;                                                       \
		}                                                                      \
		node->height =                                                         \
			__##_node_name##_max(__##_node_name##_get_height(node->left),      \
								 __##_node_name##_get_height(node->right)) +   \
			1;                                                                 \
		int balance = _name##_get_balance(node);                               \
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
	}                                                                          \
                                                                               \
	_node_name *__##_node_name##_min_node(_node_name *_head) {                 \
		_node_name *curr = _head;                                              \
		while (curr->left)                                                     \
			curr = curr->left;                                                 \
		return curr;                                                           \
	}                                                                          \
                                                                               \
	_node_name *_node_name##_remove(_node_name *root, _type key) {             \
		if (!root)                                                             \
			return root;                                                       \
                                                                               \
		if (_cmp(key, root->data) < 0)                                         \
			root->left = _node_name##_remove(root->left, key);                 \
		else if (_cmp(key, root->data) > 0)                                    \
			root->right = _node_name##_remove(root->right, key);               \
		else {                                                                 \
                                                                               \
			if (!root->left || !root->right) {                                 \
				_node_name *temp = root->left ? root->left : root->right;      \
                                                                               \
				if (!temp) {                                                   \
					temp = root;                                               \
					root = NULL;                                               \
				} else {                                                       \
					*root = *temp;                                             \
					free(temp);                                                \
				}                                                              \
			} else {                                                           \
				_node_name *temp = __##_node_name##_min_node(root->right);     \
				root->data = temp->data;                                       \
				root->right = _node_name##_remove(root->right, temp->data);    \
			}                                                                  \
		}                                                                      \
		if (!root)                                                             \
			return root;                                                       \
		root->height = 1 + __##_node_name##_max(                               \
							   __##_node_name##_get_height(root->left),        \
							   __##_node_name##_get_height(root->right));      \
		int balance = _name##_get_balance(root);                               \
                                                                               \
		if (balance > 1 && _name##_get_balance(root->left) >= 0) {             \
			return __##_node_name##_right_rotate(root);                        \
		}                                                                      \
                                                                               \
		if (balance > 1 && _name##_get_balance(root->left) < 0) {              \
			root->left = __##_node_name##_left_rotate(root->left);             \
			return __##_node_name##_right_rotate(root);                        \
		}                                                                      \
                                                                               \
		if (balance < -1 && _name##_get_balance(root->right) <= 0) {           \
			return __##_node_name##_left_rotate(root);                         \
		}                                                                      \
                                                                               \
		if (balance < -1 && _name##_get_balance(root->right) > 0) {            \
			root->right = __##_node_name##_right_rotate(root->right);          \
			return __##_node_name##_left_rotate(root);                         \
		}                                                                      \
                                                                               \
		return root;                                                           \
	}                                                                          \
                                                                               \
	bool _node_name##_contains(_node_name *root, _type key) {                  \
		if (!root) {                                                           \
			return false;                                                      \
		}                                                                      \
		if (_cmp(key, root->data) < 0)                                         \
			return _node_name##_contains(root->left, key);                     \
		else if (_cmp(key, root->data) > 0)                                    \
			return _node_name##_contains(root->right, key);                    \
		else {                                                                 \
			return true;                                                       \
		}                                                                      \
	}                                                                          \
	size_t _node_name##_size(_node_name *root) {                               \
		if (!root)                                                             \
			return 0;                                                          \
		return 1 + _node_name##_size(root->left) +                             \
			   _node_name##_size(root->right);                                 \
	}                                                                          \
                                                                               \
	_node_name *_node_name##_insert_replace(_node_name *root, _type key) {     \
		if (_node_name##_contains(root, key)) {                                \
			root = _node_name##_remove(root, key);                             \
		}                                                                      \
		root = _node_name##_insert(root, key);                                 \
		return root;                                                           \
	}

#endif