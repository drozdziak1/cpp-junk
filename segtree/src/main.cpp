#include <cstdio>

#include <iostream>

#include "SegTree.h"

void help(void)
{
	std::cout << "Here's what you can do:\n"
	          << "i a b val - increment a..b range (inclusive) by val\n"
	          << "m [a b] - query for the maximum, optionally in range a..b\n"
	          << "x - exit\n" << std::endl;
}

int main()
{
	int n;
	bool done = false;

	std::cout << "How big an array would you like the tree to represent? ";
	std::cin >> n;
	std::cout << "Great! We're working with an array of length " << n
	          << ", which now is filled with zeros." << std::endl;

	help();

	SegTree tree(n);

	// Hacky TUI, will do for testing out the tree
	do {
		char command;
		int a, b;
		long long val;

		std::cout << "Command: ";
		std::cin >> command;

		switch (command) {
		case 'i':
			std::cin >> a >> b >> val;
			std::cout << "incrementing " << a << ".." << b << " by "
			          << val << std::endl;
			tree.increment(a, b, val);
			break;
		case 'm':
			if (getchar() == ' ') {
				std::cin >> a >> b;
				std::cout << "Maximum of " << a << ".." << b
				          << ": " << tree.maximum(a, b)
				          << std::endl;
			} else {
				std::cout << "Maximum (overall): "
				          << tree.maximum() << std::endl;
			}
			break;
		case 'x':
			done = true;
			break;
		default:
			help();
		}
	} while (!done);
}
