#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

#include <ncurses.h>

#include "game.h"

Game::Game(int width, int height, int winlen)
	: brd_width_(width),
	  brd_height_(height),
	  brd_win_width_(apply_h_spacing(width)),
	  brd_win_height_(apply_v_spacing(height)),
	  brd_data_(width * height),
	  winning_length(winlen)
{
	initscr();
	win_ = stdscr;

	getmaxyx(win_, win_height_, win_width_);

	// Place the board window in the middle of the screen
	brd_win_ = subwin(
	               win_,
	               brd_win_height_, brd_win_width_,
	               (win_height_ - brd_win_height_) / 2, (win_width_ - brd_win_width_) / 2
	           );

	// Clear the board out
	std::fill(brd_data_.begin(), brd_data_.end(), Field::Free);

	keypad(brd_win_, true);
	noecho();
	curs_set(0);

	// Select the column closest to the middle
	brd_col_selected_ = brd_width_ / 2;

	// Randomly choose the starting player
	std::srand(std::time(0));
	current_player = (std::rand() % 2) ?
	                 Field::PlayerOne : Field::PlayerTwo;
}

Game::~Game()
{
	endwin();
}


int Game::redraw()
{
	int retval;
	for (int x = 0; x < brd_width_; ++x) {
		for (int y = 0; y < brd_height_; ++y) {

			if (x == brd_col_selected_)
				wattron(brd_win_, A_STANDOUT);

			retval = mvwprintw(
			             brd_win_,
			             apply_v_spacing(y),
			             apply_h_spacing(x),
			             "%c",
			             static_cast<char>(get_field(x, y))
			         );
			wattroff(brd_win_, A_STANDOUT);
			if (retval == ERR) {
				return retval;
			}
		}
	}

	wattron(win_, A_STANDOUT);

	switch (current_player) {

	case Field::PlayerOne:
		retval = mvwprintw(
		             win_,
		             3,
		             (win_width_ - strlen("Player One")) / 2,
		             "Player One"
		         );
		break;

	case Field::PlayerTwo:
		retval = mvwprintw(
		             win_,
		             3,
		             (win_width_ - strlen("Player Two")) / 2,
		             "Player Two"
		         );
		break;
	}

	wattroff(brd_win_, A_STANDOUT);
	wrefresh(win_);

	return retval;
}

void Game::step()
{
	int c = wgetch(brd_win_);

	switch (c) {

	case KEY_LEFT:
		// the extra brd_width_ makes the leftmost left move wrap around
		brd_col_selected_ = (--brd_col_selected_ + brd_width_) % brd_width_;
		break;

	case KEY_RIGHT:
		brd_col_selected_ = ++brd_col_selected_ % brd_width_;

		break;

	case KEY_ENTER:
	case 10: // Regular newline
		if (move()) {
			check_for_win();

			if (current_player == Field::PlayerOne)
				current_player = Field::PlayerTwo;
			else
				current_player = Field::PlayerOne;

		}
		break;

	case 'q':
	case 'Q':
		has_ended = true;
		return;

	default:
		mvprintw(0, 0, "Unknown key: %c %d", c, c);
	}

	redraw();
}

bool Game::move()
{
	for (int y = brd_height_ - 1; y >= 0; --y) {
		if (get_field(brd_col_selected_, y) == Field::Free) {
			set_field(brd_col_selected_, y, current_player);
			last_x = brd_col_selected_;
			last_y = y;
			return true;
		}
	}

	return false;
}

void Game::check_for_win()
{
	for (int offset = winning_length - 1; offset > 0; --offset) {
		// TODO: write the winner detection logic
	}
}
