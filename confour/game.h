#ifndef GAME_H
#define GAME_H

#include <vector>

#include <ncurses.h>

class Game
{
public:
	enum class Field {
		Free = '.',
		PlayerOne = 'O',
		PlayerTwo = 'X'
	};

	Game(int height, int width);
	Game(int size);
	virtual ~Game();
	int redraw();
	inline Field get_field(int x, int y) const;
	void set_field(int x, int y, const Field &value);

private:
	int brd_width_, brd_height_;
	int brd_win_width_, brd_win_height_;
	WINDOW *brd_win_;        // Board state drawing place
	std::vector<Field> brd_data_; // Board state values
};

#endif /* GAME_H */
