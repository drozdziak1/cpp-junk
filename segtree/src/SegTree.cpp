#include <algorithm>
#include <climits>
#include <cstdio>
#include <vector>

#include "SegTree.h"

SegTree::SegTree(int n) :
	length(n)
{
	init(&root, 1, n);
}

SegTree::~SegTree()
{
	deinit(&root);
}

/**
 * init() - builds the tree by allocating nodes
 * and setting their respective ranges.
 */
void SegTree::init(SegTreeNode *node, int a, int b)
{
	int midpoint = (a + b) / 2;

	node->low = a;
	node->high = b;

	if (a == b) // Bail on leaf nodes
		return;

	node->left = new SegTreeNode;
	init(node->left, a, midpoint);

	node->right = new SegTreeNode;
	init(node->right, midpoint + 1, b);

	node_count += 2;
}

/**
 * deinit() - traverses the tree and deallocates
 * everything starting from the leftmost leaf.
 */
void SegTree::deinit(SegTreeNode *node)
{
	if (node->left) {
		deinit(node->left);
		node->left = nullptr;
	}

	if (node->right) {
		deinit(node->right);
		node->right = nullptr;
	}

	if (node != &root)
		delete node;
}

/**
 * prop() - propagates a node's range query delta
 * value to its children down the tree.
 */
void SegTree::prop(SegTreeNode *node)
{
	// Propagate delta to the child nodes
	node->left->delta += node->delta;
	node->right->delta += node->delta;

	// Zero out the parent
	node->delta = 0;
}

/**
 * update() - updates the maximum value of a specified node.
 */
void SegTree::update(SegTreeNode *node)
{
	long long left_sum = node->left->max + node->left->delta;
	long long right_sum = node->right->max + node->right->delta;

	node->max = (left_sum > right_sum) ? left_sum : right_sum;
}

void SegTree::increment(int a, int b, int val)
{
	increment(&root, a, b, val);
}

/**
 * increment() - adds a number to a range while
 * taking advantage of lazy propagation.
 */
void SegTree::increment(SegTreeNode *node, int a, int b, int val)
{
	if (b < node->low || node->high < a)
		return;

	if (a <= node->low && node->high <= b) {
		node->delta += val;
		return;
	}

	prop(node);

	increment(node->left, a, b, val);
	increment(node->right, a, b, val);

	update(node);
}

long long SegTree::maximum(void)
{
	return maximum(&root, 0, length - 1);
}

long long SegTree::maximum(int a, int b)
{
	return maximum(&root, a, b);
}

/**
 * maximum(node, a, b) - fetches the maximum in range a..b
 */
long long SegTree::maximum(SegTreeNode *node, int a, int b)
{
	if (b < node->low || node->high < a)
		return LLONG_MIN;

	if (a <= node->low && node->high <= b)
		return node->max + node->delta;

	prop(node);

	long long max_left = maximum(node->left, a, b);
	long long max_right = maximum(node->right, a, b);

	update(node);

	return (max_left > max_right) ? max_left : max_right;
}

