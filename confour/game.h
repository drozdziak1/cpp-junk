#ifndef GAME_H
#define GAME_H

#include <vector>

#include <ncurses.h>

/**
 * A "connect four" game model
 *
 * Note: Coordinates start at top-left and are zero-indexed
 */
class Game
{
public:
	/**
	 * holds the chars representing the players' chips or
	 * a free field; guards board correctness
	 */
	enum class Field {
		Free = '.',
		PlayerOne = 'O',
		PlayerTwo = 'X'
	};

	/**
	 * h_spacing, v_spacing - how many spaces till the next field
	 * horizontally/vertically
	 */
	const static int h_spacing = 4;
	const static int v_spacing = 2;

	/**
	 * Prepares a board of the specified dimensions
	 */
	Game(int height, int width);

	/**
	 * Prepares a square board
	 */
	Game(int size) : Game(size, size) {}

	~Game();

	/**
	 * redraw() - updates brd_win_ with the latest brd_data_ values
	 * with spacing applied
	 */
	int redraw();

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
		brd_data_[x * brd_height_ + y] = value; }

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
	 * Actual board dimensions
	 */
	int brd_width_, brd_height_;

	/**
	 * Board dimensions with spacing applied (a.k.a. ncurses board window
	 * dimensions)
	 */
	int brd_win_width_, brd_win_height_;

	/**
	 * brd_win_ - contains the ncurses board visible to the user
	 */
	WINDOW *brd_win_;

	/**
	 * brd_data_ - holds the underlying Field values
	 */
	std::vector<Field> brd_data_; // Board state values
};

#endif /* GAME_H */
