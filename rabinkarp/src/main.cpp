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

void hash_step(int &hash, size_t len, char old_char, char new_char)
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
	hash += new_char * new_char_pow;
}

int main(int argc, char *argv[])
{
	int phrase_hash, current_hash, fake_matches = 0, real_matches = 0;
	size_t file_length;

	std::string phrase, filename, current_chunk;
	std::ifstream file;

	if (argc != 3) {
		std::cout << "Wrong number of arguments, using defaults"
		          << std::endl;

		filename = "rabinkarp/input.txt"; // Lorem ipsum inside
		phrase = "turp"; // Got one collision
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
		std::cerr << "Warning: Your phrase is bigger than the file!"
		          << std::endl;
	}

	std::cout << "Looking for \"" << phrase << "\" in " << filename << "..."
	          << std::endl;

	phrase_hash = hash(phrase);

	std::cout << "Phrase Hash: " << phrase_hash << std::endl;

	current_chunk.resize(phrase.length());

	file.read(&current_chunk.front(), phrase.length());

	current_hash = hash(current_chunk);
	std::cout << "Starting Chunk: \"" << current_chunk << "\"\n"
	          << "Starting Chunk Hash: " << current_hash << '\n'
	          << std::endl;

	// End of boilerplate
	do {
		char old_char, new_char;

		// Do the cheap hash check
		if (current_hash == phrase_hash) {

			std::cout << "Got same hash " << current_hash
			          << " from \"" << phrase
			          << "\" and \""
			          << current_chunk << "\" at char "
			          << ((size_t) file.tellg() - phrase.length() + 1)
			          << std::endl;

			// Fall back to the more expensive string comparison
			if (current_chunk == phrase) {
				++real_matches;
				std::cout << "It's a MATCH" << std::endl;
			} else {
				++fake_matches;
				std::cout << "It's NOT A MATCH" << std::endl;
			}

			std::cout << std::endl;
		}

		old_char = current_chunk.front();
		new_char = file.peek();

		// Recalculate the current hash
		hash_step(current_hash, phrase.length(), old_char, new_char);

		// Pop the old char and append the next one;
		current_chunk.erase(0, 1);
		current_chunk += new_char;

	} while (file.get() != EOF);

	std::cout << "Fake Matches: " << fake_matches << '\n'
	          << "Real Matches: " << real_matches
	          << std::endl;

	file.close();
	return 0;
}
