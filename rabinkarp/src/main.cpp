#include <fstream>
#include <iostream>
#include <string>

#define PRIME 3

int hash(std::string str)
{
	int ret = 0;
	int coeff = 1;

	for (char &c : str) {
		ret += c * coeff;
		coeff *= PRIME;
	}

	return ret;
}

int hash_step(int hash, int len, char old_char, char new_char)
{
	int new_char_pow = 1;

	// Detach the old character
	hash -= old_char;

	// Move rest of the hash one power down
	hash /= PRIME;

	// Build up the new power
	for (int i = 1; i < len; ++i)
		new_char_pow *= PRIME;

	// Attach the new character
	return hash + new_char * new_char_pow;
}

int main(int argc, char *argv[])
{
	char *current_chunk;
	int phrase_hash;
	size_t file_length;

	std::string phrase, filename;
	std::ifstream file;

	if (argc != 3) {
		std::cout << "Wrong number of arguments, using defaults"
		          << std::endl;

		phrase = "dog";
		filename = "rabinkarp/input.txt";
	} else {
		filename = argv[1];
		phrase = argv[2];
	}

	// Start at file end for easier length checking
	file.open(filename, std::ios::in | std::ios::ate);

	if (!file.good()) {
		std::cerr << filename << ": no such file or directory"
		          << std::endl;
		return ENOENT;
	}

	file_length = file.tellg();

	// Get back at file start
	file.seekg(0, std::ios_base::beg);

	if (phrase.length() > file_length) {
		std::cerr << "Error: Your phrase is bigger than the file!"
		          << std::endl;
		return EINVAL;
	}

	std::cout << "Looking for \"" << phrase << "\" in " << filename << "..."
	          << std::endl;

	phrase_hash = hash(phrase);

	std::cout << "Phrase Hash: " << phrase_hash << std::endl;

	current_chunk = new char[phrase.length() + 1];

	if (!current_chunk) {
		std::cerr << "Failed to allocate the phrase comparison buffer"
		          << std::endl;
		file.close();
		return 1;
	}

	file.read(current_chunk, phrase.length());
	current_chunk[phrase.length()] = '\0';

	std::cout << "Current Chunk: \"" << current_chunk << "\"\n"
	       << "Current Hash: " << hash(std::string(current_chunk)) << std::endl;

	delete[] current_chunk;
	file.close();
	return 0;
}
