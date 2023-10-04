#include "Card.h"

#ifdef BingoProject_EXPORTS
#define DL_EXPOSE __declspec(dllexport)
#else
#define DL_EXPOSE __declspec(dllimport)
#endif

struct DL_EXPOSE Message
{
	unsigned code = NULL;
	string message = nullptr;

	unsigned short num_cards = NULL;
	unsigned user_credits = NULL;
	vector<Card> cards;
	unordered_set<unsigned> drawn_balls;
};