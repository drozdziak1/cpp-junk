#include <algorithm>
#include <string>
#include <vector>

#include "GSItem.h"

char GSItem::default_name = 'A';

GSItem::GSItem()
{
	name = &default_name;
	++default_name;
}

GSItem::GSItem(std::vector<GSItem *> new_prefs) :
	prefs(new_prefs)
{
	name = &default_name;
	++default_name;
}

bool GSItem::eval_proposer(GSItem &proposer)
{
	std::cout << proposer.name << " proposes to " << name;

	// tfw no bf
	if (!spouse) {
		spouse = &proposer;
		proposer.spouse = this;
		std::cout << ", FIRST PROPOSER ACCEPTED" << std::endl;
		return true;
	}

	// Obtain proposer and spouse positions for comparison
	auto proposer_iter = std::find(prefs.begin(), prefs.end(), &proposer);
	auto spouse_iter = std::find(prefs.begin(), prefs.end(), spouse);

	if (proposer_iter < spouse_iter) {

		std::cout << " and is better than "
		          << spouse->name << ", SWAP ACCEPTED"
		          << std::endl;

		// Unburden our ex
		spouse->spouse = nullptr;

		// The man is devastated (also, now we know how many prefs to ommit)
		++spouse->failures;

		// Give the new guy our number
		proposer.spouse = this;

		// Call him boo
		spouse = &proposer;

		return true;
	}

	std::cout << " and is REJECTED in favor of "
	          << spouse->name << std::endl;

	 /*
	  * Proposer is not good enough, he won't get
	  * anywhere near her anytime soon (again,
	  * this helps with redundant proposals).
	  */
	++proposer.failures;

	return false;
}
