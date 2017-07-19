#include "game.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	Game *g;
	switch (argc) {
	case 2:
		g = new Game(std::stoi(argv[1]));
		break;
	case 3:
		g = new Game(std::stoi(argv[1]), std::stoi(argv[2]));
		break;

	case 4:
		g = new Game(
		    std::stoi(argv[1]),
		    std::stoi(argv[2]),
		    std::stoi(argv[3])
		);
		break;
	default:
		std::cerr << "Usage: " << argv[0] << " X [Y [winning_length]]"
		          << std::endl;
		exit(1);
	}

	g->redraw();

	while (!g->has_ended) {
		g->step();
	}
	getch();

	delete g;

	return 0;
}
