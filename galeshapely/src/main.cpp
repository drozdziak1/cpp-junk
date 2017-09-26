#include <algorithm>
#include <cmath>
#include <ctime>
#include <functional>
#include <iostream>
#include <set>
#include <utility>

#include "GSItem.h"

int main(int argc, char *argv[])
{
	std::srand(time(0));
	const size_t membs_per_group = 7;
	bool stable;
	int round = 1;

	std::vector<GSItem> proposers(membs_per_group);
	std::vector<GSItem> proposees(membs_per_group);

	// Helper vectors for generating random permutations
	std::vector<GSItem *> proposer_perm;
	std::vector<GSItem *> proposee_perm;

	std::function<GSItem *(GSItem &)> referencer = [](GSItem & ref) {
		return &ref;
	};

	// Fill the to-be permutations with pointers
	std::transform(proposers.begin(), proposers.end(),
	               std::back_inserter(proposer_perm), referencer);
	std::transform(proposees.begin(), proposees.end(),
	               std::back_inserter(proposee_perm), referencer);

	// Shuffle the vectors while also printing everyone out
	std::cout << "Proposers: \n";
	for (GSItem &proposer : proposers) {
		std::random_shuffle(proposee_perm.begin(), proposee_perm.end());

		proposer.prefs = proposee_perm;

		std::cout << proposer << '\n';
	}
	std::cout << '\n' << "Proposees: \n";
	for (GSItem &proposee : proposees) {
		std::random_shuffle(proposer_perm.begin(), proposer_perm.end());

		proposee.prefs = proposer_perm;

		std::cout << proposee << '\n';
	}

	std::cout << "\nMATCHING:" << std::endl;
	do {
		stable = true;
		std::cout << "ROUND " << round << ':' << std::endl;
		for (GSItem &proposer : proposers) {
			// Process only free proposers
			if (!proposer.spouse) {
				std::cout << "Proposals from "
				          << proposer << std::endl;
				for (
				    // proposer.failures help ommit the ones that said no
				    auto pref = proposer.prefs.begin() + proposer.failures;
				    pref != proposer.prefs.end();
				    ++pref
				) {
					if ((*pref)->eval_proposer(proposer)) {
						 /* Someone agreed, let's deal
						  * with the next guy
						  */
						break;
					}
				}
				std::cout << std::endl;
			}
		}
		for (GSItem &proposer : proposers) {
			if (!proposer.spouse) {
				stable = false;
				break;
			}
		}
		std::cout << std::endl;
		++round;
	} while (!stable);

	std::cout << "\nPROPOSERS AFTER MATCHING:\n";
	for (GSItem &proposer : proposers)
		std::cout << proposer << std::endl;

	std::cout << std::endl;

	return 0;
}
