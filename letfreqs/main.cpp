#include <algorithm>
#include <iostream>
#include <vector>

#include <cstdio>

using namespace std;

struct FreqCmp {
	bool operator()(auto &lhs, auto &rhs)
	{
		return lhs.second < rhs.second;
	}
};

int main(int argc, char *argv[])
{
	string input = (argc == 2) ? argv[1] : "The quick brown fox jumps over the lazy dog";
	transform(input.begin(), input.end(), input.begin(), ::tolower);

	vector<pair<char, int> > appearances;

	for (int i = 0; i < 256; ++i)
		appearances.push_back({i, 0});


	for (const char &chr : input)
		appearances[chr].second++;

	// Sort in reverse for easy resize
	sort(appearances.rbegin(), appearances.rend(), FreqCmp());

	// Get only top 10 characters
	appearances.resize(10);

	for (auto i = appearances.begin(); i != appearances.end(); ++i) {
		if (i->second == 0)
			break;

		printf("'%c'\t(%d):\t%zu\n", i->first, i->first, i->second);
	}

	return 0;
}
