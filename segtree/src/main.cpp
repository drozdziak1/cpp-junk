#include <SegTree.h>

#include <climits>
#include <cstdlib>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

auto random_init(void)
{
	// Create an RNG instance
	std::random_device rnd_device;

	// Choose the Mersenne twister
	std::mt19937 mersenne_engine(rnd_device());
	std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);

	return std::bind(dist, mersenne_engine);
}

int main(int argc, char *argv[])
{
	int n; // No. of elements

	if (argc != 2) {
		std::cout << "How many elements? ";
		std::cin >> n;
	} else {
		n = std::stoi(argv[1]);
	}

	std::cout << "Choosing " << n << " random elements: " << std::endl;


	std::vector<int> vec(n);
	std::generate(begin(vec), end(vec), random_init());

	return 0;
}
