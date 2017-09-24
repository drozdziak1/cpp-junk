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

	GSItem *spouse = nullptr;

	GSItem();
	GSItem(std::vector<GSItem *> new_prefs);

	GSItem(std::string name, std::vector<GSItem *> new_prefs) :
		name(name),
		prefs(new_prefs) {};

	bool eval_proposer(GSItem& proposer);

	friend std::ostream &operator<<(std::ostream &os, GSItem &item)
	{
		std::string preflist = item.name + ": ";
		for (auto pref : item.prefs) {
			preflist += pref->name + ' ';
		}

		if (item.spouse)
			preflist += " with spouse " + item.spouse->name;
		else
			preflist += "with no spouse";

		return os << preflist;
	}
};
#endif /* ifndef GSITEM */
