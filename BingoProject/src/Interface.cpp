#include "Interface.h"

extern "C++" {

	BingoProject_API Message generate_cards(Message msg)
	{
		Message response;
		if (msg.num_cards == 0) {
			response.code = 400;
			response.message = "Number of cards not specified.";
			return response;
		}
		response.code = 200;
		if (msg.num_cards > MAX_CARDS) {
			response.cards = std::vector(MAX_CARDS, Card());
			response.message = "Generated maximum number of cards.";
			return response;
		}
		else {
			response.cards = std::vector(msg.num_cards, Card());
			response.message = "Cards generated.";
		}
		return response;
	}

	BingoProject_API Message draw_ball(Message msg)
	{
		Message response;
		response.user_credits = msg.user_credits;

		if (msg.cards.empty()) {
			response.code = 400;
			response.message = "No cards in play.";
			return response;
		}
		// check if the game is starting to withdraw the credits from the user
		if (msg.drawn_balls.empty()) {
			if (msg.user_credits < msg.cards.size() * PRICE_PER_CARD) {
				response.code = 400;
				response.message = "Not enough credits to buy that amount of cards.";
				return response;
			}
			else {
				response.user_credits = msg.user_credits - msg.cards.size() * PRICE_PER_CARD;
			}
		}
		else if (msg.drawn_balls.size() >= N_BALLS) { // drawing extra balls
			unsigned extraBalls = msg.drawn_balls.size() + 1 - N_BALLS;
			if (extraBalls > N_MAX_EXTRABALLS) {
				response.code = 400;
				response.message = "Maximum number of extra balls reached.";
				return response;
			}
			else {
				unsigned price = extraBalls * PRICE_INCREASE_PER_EXTRABALL;
				if (msg.user_credits < price) {
					response.code = 400;
					response.message = "Not enough credits to buy an extra ball.";
					return response;
				}
				else {
					response.user_credits = msg.user_credits - price;
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
			response.code = 400;
			response.message = "No more balls to draw.";
			response.user_credits = msg.user_credits;
			return response;
		}

		std::uniform_int_distribution<unsigned> dist2(0, availableBalls.size() - 1);
		unsigned drawnBall = availableBalls[dist2(g)];

		for (unsigned i = 0; i < msg.cards.size(); i++) {
			for (unsigned j = 0; j < N_ROWS; j++) {
				for (unsigned k = 0; k < N_COLS; k++) {
					if (msg.cards[i].getNumbers()[j][k].getValue() == drawnBall) {
						msg.cards[i].markNumber(j,k);
					}
				}
			}
		}
		msg.drawn_balls.insert(drawnBall);
		response.drawn_balls = msg.drawn_balls;
		response.code = 200;


		return response;
	}

	BingoProject_API Message check_cards(Message msg)
	{
		Message response;
		response.user_credits = msg.user_credits;
		if (msg.cards.empty()) {
			response.code = 400;
			response.message = "No cards in play.";
			return response;
		}
		if (msg.drawn_balls.size() != N_BALLS) {
			response.code = 400;
			response.message = "Not all balls were drawn.";
			return response;
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
						if (row != 2) pattern2 = false;


						if (!pattern1 && !pattern2) break;
					}
				}
			}
			if (pattern1) {
				response.user_credits += PAYOUT_1;
				msg.cards[i].cashOut();
			}
			if (pattern2) {
				response.user_credits += PAYOUT_2;
				msg.cards[i].cashOut();
			}

		}
		response.message = "You lost.";
		response.code = 200;
		return response;
	}
}