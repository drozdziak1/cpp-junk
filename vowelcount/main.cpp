#include <iostream>
#include <unordered_set>

using namespace std;

int main(int argc, char *argv[])
{
	unordered_set<char> vowels = {'a', 'e', 'i', 'o', 'u', 'y'};

	unsigned vowel_count = 0;

	if (argc != 2) {
		cerr << "Wrong argument count" << endl;
		exit(1);
	}

	for (const char& chr : string(argv[1])) {
		if (vowels.find(chr) != vowels.end())
			++vowel_count;
	}

	cout << "Found " << vowel_count << " vowels";

	return 0;
}
