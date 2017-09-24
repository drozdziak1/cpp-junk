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
	// tfw no bf
	if (!spouse) {
		spouse = &proposer;
		proposer.spouse = this;
		std::cout << name << "'s first proposal from "
		          << proposer.name << ", accepting..."
		          << std::endl;
		return true;
	}

	auto proposer_iter = std::find(prefs.begin(), prefs.end(), &proposer);
	auto spouse_iter = std::find(prefs.begin(), prefs.end(), spouse);

	std::cout << "Proposer " << proposer.name << " found in " << name
	          << " at " << proposer_iter - prefs.begin() << std::endl;

	if (proposer_iter < spouse_iter) {

		std::cout << "Spouse swap in " << name << " from "
		          << spouse->name << " to " << proposer.name
		          << std::endl;

		// Unburden our ex
		spouse->spouse = nullptr;
		auto self_iter = std::find(spouse->prefs.begin(), spouse->prefs.end(), this);
		spouse->prefs.erase(self_iter, self_iter + 1);

		// Give the new guy our number
		proposer.spouse = this;

		// Call him boo
		spouse = &proposer;

		return true;
	}

	auto self_iter = std::find(proposer.prefs.begin(), proposer.prefs.end(), this);
	proposer.prefs.erase(self_iter, self_iter + 1);

	std::cout << proposer.name << " rejected by " << name << " in favor of "
	          << spouse->name << std::endl;

	return false;
}
