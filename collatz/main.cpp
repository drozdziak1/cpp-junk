#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	int num;

	if (argc != 2 ) {
		cerr << "Wrong input, using number 129" << endl;
		num = 129;
	} else {
		try {
			num = stoi(argv[1]);
		} catch (...) {
			cerr << "Couldn't parse your number, using 129" << endl;
			num = 129;
		}
	}

	int i = 0;
	while (num > 1) {
		cout << "Step\t" << i << ":\t" << num << endl;
		if (num % 2) {
			num = 3 * num + 1;
		} else {
			num /= 2;
		}
		++i;
	}
	cout << "Step\t" << i << ":\t" << num << endl;
	cout << "Took " << i << " steps, exiting..." << endl;
	return 0;
}
