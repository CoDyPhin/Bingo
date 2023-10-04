#include "Card.h"

struct Message
{
	unsigned code;
	string message;

	unsigned short num_cards;
	unsigned userCredits;
	vector<Card> cards;
	unordered_set<unsigned> drawnBalls;
};