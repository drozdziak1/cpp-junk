#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

#include "GSItem.h"

int main(int argc, char *argv[])
{
	std::srand(time(0));
	size_t membs_per_group = 10;

	std::vector<GSItem> proposers(membs_per_group);
	std::vector<GSItem> proposees(membs_per_group);

	// Helper vectors for generating random permutations
	std::vector<GSItem *> proposer_perm;
	std::vector<GSItem *> proposee_perm;

	for (auto iter = proposers.begin(); iter != proposers.end(); ++iter) {
		proposer_perm.push_back(&*iter);
	}

	for (auto iter = proposees.begin(); iter != proposees.end(); ++iter) {
		proposee_perm.push_back(&*iter);
	}

	std::cout << "Proposers: \n";
	for (GSItem& proposer : proposers) {
		std::random_shuffle(proposee_perm.begin(), proposee_perm.end());

		proposer.prefs = proposee_perm;

		std::cout << proposer << '\n';
	}

	std::cout << '\n' << "Proposees: \n";
	for (GSItem& proposee : proposees) {
		std::random_shuffle(proposer_perm.begin(), proposer_perm.end());

		proposee.prefs = proposer_perm;

		std::cout << proposee << '\n';
	}

	for (GSItem &proposer : proposers) {
		for (auto iter = proposer.prefs.begin(); iter != proposer.prefs.end(); ++iter) {
			if ((*iter)->eval_proposer(proposer))
				break;
		}
	}

	for (GSItem &proposer : proposers) {
/*
 *                std::cout << proposer.name << ": ";
 *
 *                if (proposer.spouse)
 *                        std::cout << proposer.spouse->name;
 *                else
 *                        std::cout << "None";
 *
 *                std::cout << std::endl;
 */

		std::cout << proposer << std::endl;
	}

	std::cout << std::endl;

	return 0;
}
