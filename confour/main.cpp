#include <game.h>
#include <string>

int main(int argc, char *argv[])
{
	Game g(std::stoi(argv[1]));
	g.set_field(0, 3, Game::Field::PlayerTwo);
	g.redraw();

	return 0;
}
