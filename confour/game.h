#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include <ncurses.h>

/**
 * A "connect four" game model
 *
 * Note: Coordinates start at top-left and are zero-indexed
 *
 * TODO:
 * - A legend
 */
class Game
{
public:
	/**
	 * holds the chars representing the players' chips or
	 * a free field; guards board correctness; also used for telling players
	 * apart
	 */
	enum class Field {
		Free = '.',
		PlayerOne = 'O',
		PlayerTwo = 'X'
	};

	/**
	 * h_spacing, v_spacing - how many spaces till the next field
	 * horizontally/vertically?
	 */
	const static int h_spacing = 4;
	const static int v_spacing = 2;

	bool has_ended = false;
	std::string msg = ""; // The message box buffer

	/**
	 * winning_length - how many fields does the winning line take?
	 */
	const int winning_length;

	/**
	 * current_player - who's turn is it?
	 */
	Field current_player;

	/**
	 * last_x, last_y - where's the last move?
	 */
	int last_x = -1, last_y = -1;

	/**
	 * Prepares a board of the specified dimensions and winning length
	 */
	Game(int width, int height, int winlen = 4);

	/**
	 * Prepares a square board
	 */
	Game(int size) : Game(size, size) {}

	/**
	 * Prepares a classic 7x6 board
	 */
	Game() : Game(7, 6) {}

	~Game();

	/**
	 * redraw() - updates brd_win_ with the latest brd_data_ values
	 * with spacing applied
	 */
	int redraw();

	/**
	 * step() - lets a player make a move and checks if they won
	 */
	void step();

	/**
	 * move() - adds the current player's token to the selected column,
	 * returns true on success and false if there's no space for the move
	 */
	bool move();

	/**
	 * check_for_win() - Checks if the current player has won
	 */
	bool check_for_win();

	/**
	 * get_field() - retrieves a Field value from brd_data_
	 */
	inline Field get_field(int x, int y) const
	{
		return brd_data_[x * brd_height_ + y];
	}

	/**
	 * You wouldn't believe what this one does
	 */
	inline void set_field(int x, int y, const Field &value)
	{
		brd_data_[x * brd_height_ + y] = value;
	}

	/**
	 * apply_*_spacing() - applies the horizontal
	 * or vertical spacing values to x/y
	 */
	inline int apply_h_spacing(int x) const
	{
		return h_spacing * (x + 1) - 1;
	}

	inline int apply_v_spacing(int y) const
	{
		return v_spacing * (y + 1) - 1;
	}

private:
	/**
	 * Game window dimensions
	 */
	int win_width_, win_height_;

	/**
	 * win_ - the ncurses window for the whole game
	 */
	WINDOW *win_;

	/**
	 * Board dimensions with spacing applied (a.k.a. ncurses board window
	 * dimensions)
	 */
	int brd_win_width_, brd_win_height_;

	/**
	 * brd_win_ - ncurses board window visible to the user
	 */
	WINDOW *brd_win_;

	/**
	 * Actual board dimensions
	 */
	int brd_width_, brd_height_;

	/**
	 * brd_data_ - holds the underlying Field values
	 */
	std::vector<Field> brd_data_; // Board state values

	/**
	 * Message box dimensions
	 */
	int msg_width_ = 50, msg_height_ = 1;

	/**
	 * Message box ncurses window
	 */
	WINDOW *msg_win_;

	/**
	 * brd_col_selected_ - the column chosen by the current player
	 */
	int brd_col_selected_;
};

#endif /* GAME_H */
