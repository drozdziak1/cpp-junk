#include <cassert>
#include <vector>

#include <ncurses.h>

#include <game.h>

Game::Game(int width, int height)
	: brd_width_(width),
	  brd_height_(height),
	  brd_win_width_(4 * width + 3),
	  brd_win_height_(2 * height + 1),
	  brd_data_(width * height)
{
	int max_x, max_y;

	assert(width > 0);
	assert(height > 0);

	initscr();

	getmaxyx(stdscr, max_y, max_x);

	// Place a window in the middle of the current terminal
	brd_win_ = subwin(
	               stdscr,
	               brd_win_height_, brd_win_width_,
	               (max_y - brd_win_height_) / 2, (max_x - brd_win_width_) / 2
	           );

	std::fill(brd_data_.begin(), brd_data_.end(), Field::Free);

	keypad(brd_win_, true);
	noecho();
	curs_set(0);

	redraw();
}

Game::Game(int size) : Game(size, size) {}

Game::~Game()
{
	getch();
	endwin();
}

int Game::redraw()
{
	int retval;
	for (int y = 0; y < brd_height_; ++y) {
		for (int x = 0; x < brd_width_; ++x) {
			retval = mvwprintw(
			             brd_win_,
			             2 * y + 1,
			             4 * x + 3,
			             "%c",
			             static_cast<char>(get_field(x, y))
			         );
			if (retval == ERR) {
				return retval;
			}
		}
	}
	return retval;
}

inline Game::Field Game::get_field(int x, int y) const
{
	return brd_data_[x * brd_width_ + y];
}

inline void Game::set_field(int x, int y, const Field &value)
{
	brd_data_[x * brd_width_ + y] = value;
}
