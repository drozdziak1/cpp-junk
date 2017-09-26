#ifndef GSITEM
#define GSITEM

#include <iostream>
#include <string>
#include <vector>

struct GSItem
{
	static char default_name;

	std::string name;
	std::vector<GSItem *> prefs;
	int failures = 0;

	GSItem *spouse = nullptr;

	GSItem();
	GSItem(std::vector<GSItem *> new_prefs);

	GSItem(std::string name, std::vector<GSItem *> new_prefs) :
		name(name),
		prefs(new_prefs) {};

	/**
	 * eval_proposer - takes proposer and makes them this->spouse
	 * if they're good enough.
	 */
	bool eval_proposer(GSItem& proposer);

	friend std::ostream &operator<<(std::ostream &os, GSItem &item)
	{
		std::string preflist = item.name + ": ";
		for (auto pref : item.prefs) {
			preflist += pref->name + ' ';
		}

		if (item.spouse)
			preflist += "with spouse " + item.spouse->name + ' ';
		else
			preflist += "with no spouse ";

		return os << preflist << '(' << item.failures << " failures)";
	}
};
#endif /* ifndef GSITEM */
