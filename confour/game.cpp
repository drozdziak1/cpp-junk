#include <cassert>
#include <vector>

#include <ncurses.h>

#include "game.h"

Game::Game(int width, int height)
	: brd_width_(width),
	  brd_height_(height),
	  brd_win_width_(apply_h_spacing(width)),
	  brd_win_height_(apply_v_spacing(height)),
	  brd_data_(width * height)
{
	int max_x, max_y;

	initscr();

	getmaxyx(stdscr, max_y, max_x);

	// Place the board window in the middle of the screen
	brd_win_ = subwin(
	               stdscr,
	               brd_win_height_, brd_win_width_,
	               (max_y - brd_win_height_) / 2, (max_x - brd_win_width_) / 2
	           );

	// Clear the board out
	std::fill(brd_data_.begin(), brd_data_.end(), Field::Free);

	keypad(brd_win_, true);
	noecho();
	curs_set(0);

	redraw();
}

Game::~Game()
{
	getch();
	endwin();
}


int Game::redraw()
{
	int retval;
	for (int x = 0; x < brd_width_; ++x) {
		for (int y = 0; y < brd_height_; ++y) {
			retval = mvwprintw(
			             brd_win_,
			             apply_v_spacing(y),
			             apply_h_spacing(x),
			             "%c",
			             static_cast<char>(get_field(x, y))
			         );
			if (retval == ERR) {
				return retval;
			}
		}
	}

	touchwin(stdscr);
	wrefresh(brd_win_);

	return retval;
}
