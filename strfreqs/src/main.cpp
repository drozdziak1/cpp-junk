#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main()
{
	int string_count, queries;

	std::cin >> string_count;

	std::unordered_map<std::string, int> string_map;

	for (int i = 0; i < string_count; ++i) {
		std::string current_string;

		std::cin >> current_string;

		auto search = string_map.find(current_string);
		if (search == string_map.end()) {
			string_map[current_string] = 0;
		} else {
			string_map[current_string] += 1;
		}
	}

	std::cin >> queries;

	for (int i = 0; i < queries; ++i) {
		std::string current_query;

		std::cin >> current_query;

		auto search = string_map.find(current_query);
		if (search == string_map.end())
			std::cout << 0;
		else
			std::cout << search->second;
	}

	return 0;
}
