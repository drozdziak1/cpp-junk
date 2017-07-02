#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	int input;

	if (argc != 2 ) {
		cerr << "Wrong input, using number 129";
		input = 129;
	} else {
		input = stoi(argv[1]);
	}

	while (input != 1) {

	}
	return 0;
}
