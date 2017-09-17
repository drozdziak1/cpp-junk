#include <climits>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#define CHLD_L(i) (2 * (i) + 1)
#define CHLD_R(i) (2 * (i) + 2)

class SegTree
{
public:
	SegTree(unsigned long long n);
	~SegTree();
	void increment (unsigned long long i, unsigned long long a, unsigned long long b, unsigned long long val);
	unsigned long long  maximum(void);
	unsigned long long maximum(unsigned long long a, unsigned long long b);

private:
	unsigned long long *lows, *highs, *maxes, *deltas;
	unsigned long long element_count;

	unsigned long long maximum(unsigned long long i, unsigned long long a, unsigned long long b);
	void init(unsigned long long i, unsigned long long a, unsigned long long b);
	void prop(unsigned long long i);
	void update(unsigned long long i);
};

SegTree::SegTree(unsigned long long n) :
	element_count(n)
{
	lows = new unsigned long long[4 * n + 1];
	highs = new unsigned long long[4 * n + 1];
	maxes = new unsigned long long[4 * n + 1];
	deltas = new unsigned long long[4 * n + 1];

	init(0, 0, n - 1);
}

SegTree::~SegTree()
{
	delete[] lows;
	delete[] highs;
	delete[] maxes;
	delete[] deltas;
}

void SegTree::prop(unsigned long long i)
{
	// Propagate delta to the child nodes
	deltas[CHLD_L(i)] += deltas[i];
	deltas[CHLD_R(i)] += deltas[i];

	// Zero out the parent
	deltas[i] = 0;
}

void SegTree::update(unsigned long long i)
{
	unsigned long long left_sum = maxes[CHLD_L(i)] + deltas[CHLD_L(i)];
	unsigned long long right_sum = maxes[CHLD_R(i)] + deltas[CHLD_R(i)];

	maxes[i] = (left_sum > right_sum) ? left_sum : right_sum;
}

void SegTree::increment(unsigned long long i, unsigned long long a, unsigned long long b, unsigned long long val)
{
	if (b < lows[i] || highs[i] < a)
		return;

	if (a <= lows[i] && highs[i] <= b) {
		deltas[i] += val;
		return;
	}

	prop(i);

	increment(CHLD_L(i), a, b, val);
	increment(CHLD_R(i), a, b, val);

	update(i);
}

unsigned long long SegTree::maximum(void)
{
	return maximum(0, 0, element_count - 1);
}

unsigned long long SegTree::maximum(unsigned long long a, unsigned long long b)
{
	return maximum(0, a, b);
}

unsigned long long SegTree::maximum(unsigned long long i, unsigned long long a, unsigned long long b)
{
	if (b < lows[i] || highs[i] < a)
		return 0;

	if (a <= lows[i] && highs[i] <= b)
		return maxes[i] + deltas[i];

	prop(i);

	unsigned long long max_left = maximum(CHLD_L(i), a, b);
	unsigned long long max_right = maximum(CHLD_R(i), a, b);

	update(i);

	return (max_left > max_right) ? max_left : max_right;
}

void SegTree::init(unsigned long long i, unsigned long long a, unsigned long long b)
{
	lows[i] = a;
	highs[i] = b;

	if (a == b)
		return;

	unsigned long long m = (a + b) / 2;

	init(2 * i + 1, a, m);
	init(2 * i + 2, m + 1, b);
}


int main()
{

	unsigned long long k;
	unsigned long a, b, n, m;
	std::cin >> n >> m;
	SegTree tree(n);

	for (unsigned long long i = 0; i < m; ++i) {
		std::cin >> a >> b >> k;

		tree.increment(0, a - 1, b - 1, k);
	}

	std::cout << tree.maximum();
	return 0;
}
