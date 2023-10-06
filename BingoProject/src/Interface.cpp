#include "Interface.h"

extern "C++" {

	BingoProject_API Message generate_cards(Message msg)
	{
		msg.message = "";
		msg.code = 500;
		if (msg.num_cards == 0) {
			msg.code = 400;
			msg.message = "Number of cards not specified.";
			return msg;
		}
		msg.code = 200;
		if (msg.num_cards > MAX_CARDS) {
			msg.cards = std::vector(MAX_CARDS, Card());
			msg.message = "Generated maximum number of cards.";
			return msg;
		}
		else {
			msg.cards = std::vector(msg.num_cards, Card());
			msg.message = "Cards generated.";
		}
		return msg;
	}

	BingoProject_API Message draw_ball(Message msg)
	{
		msg.message = "";
		msg.code = 500;
		if (msg.cards.empty()) {
			msg.code = 400;
			msg.message = "No cards in play.";
			return msg;
		}
		// check if the game is starting to withdraw the credits from the user
		if (msg.drawn_balls.empty()) {
			if (msg.user_credits < msg.cards.size() * PRICE_PER_CARD) {
				msg.code = 403;
				msg.message = "Not enough credits to buy that amount of cards.";
				return msg;
			}
			else {
				msg.user_credits = msg.user_credits - msg.cards.size() * PRICE_PER_CARD;
			}
		}
		else if (msg.drawn_balls.size() >= N_BALLS) { // drawing extra balls
			unsigned extraBalls = msg.drawn_balls.size() + 1 - N_BALLS;
			if (extraBalls > N_MAX_EXTRABALLS) {
				msg.code = 202;
				msg.message = "Maximum number of extra balls reached.";
				return msg;
			}
			else {
				unsigned const price = extraBalls * PRICE_INCREASE_PER_EXTRABALL;
				if (msg.user_credits < price) {
					msg.code = 202;
					msg.message = "Not enough credits to buy an extra ball.";
					return msg;
				}
				else {
					msg.user_credits = msg.user_credits - price;
				}
			}
		}
		// draw a ball
		std::random_device rd;
		std::mt19937 g(rd());
		std::uniform_int_distribution<unsigned> dist(MIN_BALL_NUM, MAX_BALL_NUM);
		std::vector<unsigned> availableBalls;
		for (int i = MIN_BALL_NUM; i <= MAX_BALL_NUM; i++) {
			// if (msg.drawn_balls.find(i) != msg.drawn_balls.end()) availableBalls.push_back(i); C++ under 20
			if (!msg.drawn_balls.contains(i)) availableBalls.push_back(i); // C++20
		}

		if (availableBalls.empty()) {
			msg.code = 400;
			msg.message = "No more balls to draw."; // this should never happen, unless someone cheated
			return msg;
		}

		std::uniform_int_distribution<unsigned> dist2(0, availableBalls.size() - 1);
		unsigned const drawnBall = availableBalls[dist2(g)];

		for (Card& c : msg.cards) {
			for (unsigned j = 0; j < N_ROWS; j++) {
				for (unsigned k = 0; k < N_COLS; k++) {
					if (c.getNumbers()[j][k].getValue() == drawnBall) {
						c.markNumber(j,k);
					}
				}
			}
		}
		msg.drawn_balls.insert(drawnBall);
		msg.message = "Ball number " + std::to_string(drawnBall) + " was drawn.";
		msg.code = 200;
		return msg;
	}

	BingoProject_API Message check_cards(Message msg)
	{
		msg.message = "";
		msg.code = 500;
		unsigned const startingCredits = msg.user_credits;
		if (msg.cards.empty()) {
			msg.code = 400;
			msg.message = "No cards in play.";
			return msg;
		}
		if (msg.drawn_balls.size() < N_BALLS) {
			msg.code = 406;
			msg.message = "Not all balls were drawn.";
			return msg;
		}
		for (unsigned i = 0; i < msg.cards.size(); i++) {
			if (msg.cards[i].isCashedOut()) continue;
			// Pattern Identification (ONLY WORKS FOR 3x5 CARDS)
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
		startingCredits >= msg.user_credits ? msg.message = "You lost." : msg.message = "You won " + std::to_string(msg.user_credits - startingCredits) + " credits.";
		msg.code = 200;
		return msg;
	}
}