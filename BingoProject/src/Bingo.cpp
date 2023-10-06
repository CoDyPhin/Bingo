#include "Bingo.h"

extern "C++" {

	BingoProject_API Message generate_cards(Message msg)
	{
		msg.message = "";
		msg.code = 500;
		if (msg.num_cards <= 0) {
			msg.code = 400;
			msg.message = "Invalid or not specified number of cards.";
			return msg;
		}
		msg.code = 200;
		if (msg.num_cards > MAX_CARDS) {
			msg.cards = std::vector(MAX_CARDS, Card());
			msg.message = "Generated maximum number of cards '" + std::to_string(MAX_CARDS) + "'.";
			return msg;
		}
		else {
			msg.cards = std::vector(msg.num_cards, Card());
			msg.message = "Generated '" + std::to_string(msg.num_cards) + "' card(s).";
		}
		msg.price = msg.cards.size() * PRICE_PER_CARD;
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
		// Check if the game is starting to withdraw the credits from the user
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
			unsigned const extraBalls = msg.drawn_balls.size() + 1 - N_BALLS;
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
					msg.message = "Extra ball bought for " + std::to_string(price) + " credit(s).\n";
				}
			}
		}
		get_ball(msg);
		int const nextBall = msg.drawn_balls.size() + 1 - N_BALLS;
		if (nextBall < 0 || nextBall > N_MAX_EXTRABALLS) msg.price = 0;
		else msg.price = (nextBall * PRICE_INCREASE_PER_EXTRABALL);
		return msg;
	}

	BingoProject_API Message check_cards(Message msg)
	{
		msg.message = "";
		msg.code = 500;
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
		check_patterns(msg);
		return msg;
	}
}