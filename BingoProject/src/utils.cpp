#include "utils.h"

void get_ball(Message& msg)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution dist(MIN_BALL_NUM, MAX_BALL_NUM);
	std::vector<unsigned> availableBalls;
	for (unsigned i = MIN_BALL_NUM; i <= MAX_BALL_NUM; i++) {
		// if (msg.drawn_balls.find(i) != msg.drawn_balls.end()) availableBalls.push_back(i); C++ under 20
		if (!msg.drawn_balls.contains(i)) availableBalls.push_back(i); // C++20
	}
	if (availableBalls.empty()) {
		msg.code = 400;
		msg.message = "No more balls to draw."; // If this happens, game consts are wrong
		return;
	}

	std::uniform_int_distribution<unsigned> dist2(0, availableBalls.size() - 1);
	unsigned const drawnBall = availableBalls[dist2(g)];

	for (Card& c : msg.cards) {
		for (unsigned j = 0; j < N_ROWS; j++) {
			for (unsigned k = 0; k < N_COLS; k++) {
				if (c.getNumbers()[j][k].getValue() == drawnBall) {
					c.markNumber(j, k);
				}
			}
		}
	}
	msg.drawn_balls.insert(drawnBall);
	msg.message += "Ball number " + std::to_string(drawnBall) + " was drawn.";
	msg.code = 200;
}


void check_patterns(Message& msg)
{
	unsigned const startingCredits = msg.user_credits;
	for (unsigned i = 0; i < msg.cards.size(); i++) {
		if (msg.cards[i].isCashedOut()) continue;

		// Pattern Identification (PATTERNS DEFINED FOR 3x5 CARDS)
		bool pattern1 = true;
		bool pattern2 = true;

		for (unsigned row = 0; row < N_ROWS; row++) {
			for (unsigned col = 0; col < N_COLS; col++) {
				if (!msg.cards[i].getNumbers()[row][col].isMarked()) {
					// Pattern 1
					if (col == 2) pattern1 = false;
					if (row == 0) pattern1 = false;
					if (row == 1 && (col == 1 || col == 3)) pattern1 = false;

					// Pattern 2
					if (col == 2) pattern2 = false;
					if (row != 1) pattern2 = false;


					if (!pattern1 && !pattern2) break;
				}
			}
		}
		if (pattern1) {
			if (!msg.cards[i].isCashedOut(1))
			{
				msg.user_credits += PAYOUT_1;
				msg.cards[i].cashOut(1);
			}
		}
		if (pattern2) {
			if (!msg.cards[i].isCashedOut(2))
			{
				msg.user_credits += PAYOUT_2;
				msg.cards[i].cashOut(2);
			}
		}
	}
	startingCredits >= msg.user_credits ? msg.message = "No prize!" : msg.message = "You won " + std::to_string(msg.user_credits - startingCredits) + " credits!";
	msg.code = 200;
}