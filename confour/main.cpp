#include "game.h"

#include <iostream>
#include <memory>
#include <string>

int main(int argc, char *argv[])
{
	std::unique_ptr<Game> g;
	switch (argc) {
	case 1:
		g = std::make_unique<Game>();
		break;
	case 2:
		g = std::make_unique<Game>(std::stoi(argv[1]));
		break;
	case 3:
		g = std::make_unique<Game>(
		        std::stoi(argv[1]),
		        std::stoi(argv[2])
		    );
		break;

	case 4:
		g = std::make_unique<Game> (
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

	return 0;
}
