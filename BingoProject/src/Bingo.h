#pragma once

#include "utils.h"

extern "C++" {

	// Generates a specified number of Bingo cards
	BingoProject_API Message generate_cards(Message msg);

	// Draws a bingo ball from the remaining balls in the pool
	BingoProject_API Message draw_ball(Message msg);

	// Checks all cards for a set of winning patterns
	BingoProject_API Message check_cards(Message msg);
}