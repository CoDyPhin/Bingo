#pragma once

#include "Card.h"

struct BingoProject_API Message
{
	unsigned code = 0;
	string message;

	unsigned short num_cards = 0;
	unsigned user_credits = 0;
	vector<Card> cards;
	unordered_set<unsigned> drawn_balls;
};