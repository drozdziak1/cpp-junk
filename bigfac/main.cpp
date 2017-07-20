#include <gmpxx.h>

#include <iostream>

int main(int argc, char *argv[])
{
	mpz_class input;
	mpz_class output = 1;

	if (argc == 2) {
		std::cout << "Proceeding with " << input.get_str() << '!'
		          << std::endl;

		input = mpz_class(argv[1]);
	} else {
		std::cout << "Wrong number of arguments, doing 100!"
		          << std::endl;

		input = mpz_class(100);
	}

	for (mpz_class i = 2; i <= input; ++i)
		output *= i;



	std::cout << input.get_str() << '!' << " is equal to:" << '\n'
		<< output.get_str() << std::endl;

	return 0;
}
