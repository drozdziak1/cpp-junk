#ifndef SEGTREE_H

/**
 * Given the time/space tradeoff I'm going with space efficiency.
 * An alternative array-based approach is also very popular,
 * but fails to use an exact number of necessary nodes
 * in exchange for faster access times.
 */
struct SegTreeNode {
	SegTreeNode *left = nullptr;
	SegTreeNode *right = nullptr;

	// Note: low and high are inclusive
	int low = 0;
	int high = 0;

	long long max = 0;
	long long delta = 0;
};

class SegTree
{
public:
	int length;
	int node_count = 1;

	SegTree(int n);
	~SegTree();
	void increment(SegTreeNode *node, int a, int b, int val);
	void increment(int a, int b, int val);
	long long maximum(void);
	long long maximum(int a, int b);

private:
	SegTreeNode root;

	long long maximum(SegTreeNode *node, int a, int b);
	void init(SegTreeNode *node, int a, int b);
	void deinit(SegTreeNode *node);
	void prop(SegTreeNode *node);
	void update(SegTreeNode *node);
};

#define SEGTREE_H
#endif /* ifndef SEGTREE_H */

