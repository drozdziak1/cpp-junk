#include <ncurses.h>

#include <string>

// Currently, this thing only prints input in the middle of the screen
int main(int argc, char *argv[])
{
	int rows, cols;
	int c;

	std::string input;
	const std::string exit_help = "Press 'x' to exit";

	initscr();

	do {
		clear();
		getmaxyx(stdscr, rows, cols);

		mvprintw(rows - 1, 0, "This terminal has %d rows and %d columns", rows, cols);
		mvprintw(rows - 1, cols - exit_help.size(), exit_help.c_str());

		mvprintw(rows / 2, (cols - input.size()) / 2, input.c_str());

		refresh();

		c = getch();

		input.push_back(c);

	} while (c != 'x');

	endwin();

	return 0;
}
