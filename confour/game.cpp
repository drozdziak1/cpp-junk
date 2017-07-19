#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
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
	start_color();
	use_default_colors();

	win_ = stdscr;

	getmaxyx(win_, win_height_, win_width_);

	// Place the board window in the middle of the screen
	brd_win_ = subwin(
	               win_,
	               brd_win_height_, brd_win_width_,
	               (win_height_ - brd_win_height_) / 2, (win_width_ - brd_win_width_) / 2
	           );

	// Clear the board
	std::fill(brd_data_.begin(), brd_data_.end(), Field::Free);

	// Enable KEY_* macros in getch()
	keypad(brd_win_, true);

	// Hide the cursor and user input
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

	init_pair(1, COLOR_BLUE, -1);
	init_pair(2, COLOR_YELLOW, -1);

	for (int x = 0; x < brd_width_; ++x) {
		for (int y = 0; y < brd_height_; ++y) {

			if (x == brd_col_selected_) {
				wattron(brd_win_, A_STANDOUT);
				wattron(brd_win_, COLOR_PAIR(1));
			}

			if (x == last_x && y == last_y)
				wattron(brd_win_, COLOR_PAIR(2));


			retval = mvwprintw(
			             brd_win_,
			             apply_v_spacing(y),
			             apply_h_spacing(x),
			             "%c",
			             static_cast<char>(get_field(x, y))
			         );
			wattroff(brd_win_, A_STANDOUT);
			wattroff(brd_win_, COLOR_PAIR(1));
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
	wrefresh(brd_win_);

	return retval;
}

void Game::step()
{
	int c = wgetch(brd_win_);
	std::string winner_msg;

	switch (c) {

	case KEY_LEFT:
		// the extra brd_width_ makes the leftmost left move wrap around
		brd_col_selected_ = (--brd_col_selected_ + brd_width_) % brd_width_;
		break;

	case KEY_RIGHT:
		brd_col_selected_ = ++brd_col_selected_ % brd_width_;

		break;

	case KEY_ENTER:
	case ' ':
	case 10: // Regular newline
		if (move()) {
			// If the move was successful, check if they won
			if (check_for_win()) {

				has_ended = true;

				winner_msg = (current_player == Field::PlayerOne) ?
				             "Player One wins" : "Player Two wins";

				wattron(win_, A_BLINK);
				mvwprintw(
				    win_,
				    win_height_ - 1,
				    (win_width_ - winner_msg.size()) / 2,
				    winner_msg.c_str()
				);
				wattroff(win_, A_BLINK);

			} else {
				current_player = (current_player == Field::PlayerOne) ?
				                 Field::PlayerTwo : Field::PlayerOne;
			}
		} else {
			int x;
			for (x = 0; x < brd_width_; ++x) {
				if (get_field(x, 0) == Field::Free)
					break;
			}

			if (x == brd_width_) {
				mvwprintw( win_, 0, 0, "We're out of space!");
			}
		}
		break;

	case 'q':
	case 'Q':
		has_ended = true;
		break;

	default:
		mvwprintw(
		    win_,
		    0,
		    0,
		    "Unknown key: %c %d", c, c
		);
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

bool Game::check_for_win()
{

	int top_left = 0, left =  0, bottom_left = 0;
	int top_right = 0, right = 0, bottom_right = 0;
	int top = 0, bottom = 0;

	/**
	 * Each loop goes in a straight line away from the last move in every
	 * direction. Each one stops once it finds anything different than the
	 * current player's piece, or if an edge of the board is reached.
	 */

	// top
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_y - i >= 0
		    && get_field(last_x, last_y - i) == current_player
		) {
			++top;
		} else {
			break;
		}
	}

	// top_right
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x + i <= brd_width_
		    && last_y - i >= 0
		    && get_field(last_x + i, last_y - i) == current_player
		) {
			++top_right;
		} else {
			break;
		}
	}

	// right
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x + i <= brd_width_
		    && get_field(last_x + i, last_y) == current_player
		) {
			++right;
		} else {
			break;
		}
	}

	// bottom_right
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x + i <= brd_width_
		    && last_y + i <= brd_height_
		    && get_field(last_x + i, last_y + i) == current_player
		) {
			++bottom_right;
		} else {
			break;
		}
	}

	// bottom
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_y + i <= brd_height_
		    && get_field(last_x, last_y + i) == current_player
		) {
			++bottom;
		} else {
			break;
		}
	}

	// bottom_left
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x - i >= 0
		    && last_y + i <= brd_height_
		    && get_field(last_x - i, last_y + i) == current_player
		) {
			++bottom_left;
		} else {
			break;
		}
	}

	// left
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x - i >= 0
		    && get_field(last_x - i, last_y) == current_player
		) {
			++left;
		} else {
			break;
		}
	}

	// top_left
	for (int i = 1; i < winning_length; ++i) {
		if (
		    last_x - i >= 0
		    && last_y - i >= 0
		    && get_field(last_x - i, last_y - i) == current_player
		) {
			++top_left;
		} else {
			break;
		}
	}

	/**
	 * The current player is a winner if we've counted at least
	 * winning_length fields from any two opposite
	 * directions; the "+ 1" bit accounts
	 * for the last move itself
	 */
	return top + bottom + 1 >= winning_length
	       || top_right + bottom_left + 1 >= winning_length
	       || right + left + 1 >= winning_length
	       || bottom_right + top_left + 1 >= winning_length;
}
