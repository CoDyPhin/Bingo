#include "Bingo.h"

// Definition of the functions that will be exported by the library

extern "C++" {

	// Generates a specified number of Bingo cards and calculates the price to play them

	// @param msg: Message object containing GameState information. In this case, the only required information is the number of cards to generate

	// @return Message object containing the generated cards and the price to play them. Additionally, a message and a code are returned.

	// @note Possible message codes:

	// Success Codes (200-299):
	//	200 (OK) - Everything worked correctly;

	// Client Error Codes (400-499):
	//	400 (Bad Request) - The message received an invalid number of cards to generate;

	// Server Error Codes (500-599):
	//	500 (Internal Server Error)

	BingoProject_API Message generate_cards(Message msg)
	{
		// Clear the message string and set the code to 500 (Internal Server Error) in case something goes wrong
		msg.message = "";
		msg.code = 500;

		// Check if the number of cards is valid
		if (msg.num_cards <= 0) {
			msg.code = 400; // Bad Request
			msg.message = "Invalid or not specified number of cards.";
			return msg;
		}
		// Check if the number of cards is greater than the maximum allowed
		if (msg.num_cards > MAX_CARDS) {
			// If it is, generate the maximum number of cards allowed instead
			msg.cards = std::vector(MAX_CARDS, Card());
			msg.num_cards = MAX_CARDS;
			msg.message = "Generated maximum number of cards '" + std::to_string(MAX_CARDS) + "'.";
			return msg;
		}
		else {
			// Generate the specified number of cards
			msg.cards = std::vector(msg.num_cards, Card());
			msg.message = "Generated '" + std::to_string(msg.num_cards) + "' card(s).";
		}
		// Calculate the price to play the generated cards
		msg.price = msg.cards.size() * PRICE_PER_CARD;
		msg.code = 200; // OK
		return msg;
	}


	// Draws a ball from the remaining balls in the pool. Can be used to draw the initial balls, or to draw extra balls

	// @param msg: Message object containing GameState information. In this case, the required informations are the number of user credits,
	//the cards in play, and the balls that were previously drawn

	// @return Message object containing the updated GameState information. This includes the updated user credits, the updated drawn balls,
	//and updated (marked) cards. Additionally, a message and a code are returned.

	// @note Possible message codes:

	// Success Codes (200-299):
	//	200 (OK) - Everything worked correctly;
	//	202 (Accepted, but not acted upon) - The user is not allowed to draw more balls;

	// Client Error Codes (400-499):
	//	400 (Bad Request) - The message had invalid information (No cards in play);
	//	403 (Forbidden) - The user does not have enough credits to buy the cards;

	// Server Error Codes (500-599):
	// 500 (Internal Server Error)

	BingoProject_API Message draw_ball(Message msg)
	{
		// Clear the message string and set the code to 500 (Internal Server Error) in case something goes wrong
		msg.message = "";
		msg.code = 500;

		// Check if there are cards in play
		if (msg.cards.empty()) {
			msg.code = 400; // Bad Request
			msg.message = "No cards in play.";
			return msg;
		}
		// Check if the game is starting to withdraw the credits from the user
		if (msg.drawn_balls.empty()) {
			// Check if the user has enough credits to buy the cards
			if (msg.user_credits < msg.cards.size() * PRICE_PER_CARD) { 
				msg.code = 403; // Forbidden
				msg.message = "Not enough credits to buy that amount of cards.";
				return msg;
			}
			else {
				// If the user has enough credits, withdraw them from the user
				msg.user_credits = msg.user_credits - msg.cards.size() * PRICE_PER_CARD;
			}
		}
		// Check if the user is trying to buy extra balls
		else if (msg.drawn_balls.size() >= N_BALLS) {
			// Calculate the number of the extra ball
			unsigned const extraBalls = msg.drawn_balls.size() + 1 - N_BALLS;
			// Check if the user is allowed to buy more extra balls
			if (extraBalls > N_MAX_EXTRABALLS) {
				msg.code = 202; // Accepted, but not acted upon
				msg.message = "Maximum number of extra balls reached.";
				return msg;
			}
			else {
				// The user is allowed to buy more extra balls, so check if he has enough credits to buy it
				unsigned const price = extraBalls * PRICE_INCREASE_PER_EXTRABALL;
				if (msg.user_credits < price) {
					msg.code = 202; // Accepted, but not acted upon
					msg.message = "Not enough credits to buy an extra ball.";
					return msg;
				}
				else {
					// If the user has enough credits, withdraw them from the user
					msg.user_credits = msg.user_credits - price;
					msg.message = "Extra ball bought for " + std::to_string(price) + " credit(s).\n";
				}
			}
		}
		// Generate the ball and update the GameState information in msg
		get_ball(msg);

		// Calculate the next ball's price
		int const nextBall = msg.drawn_balls.size() + 1 - N_BALLS;
		if (nextBall < 0 || nextBall > N_MAX_EXTRABALLS) msg.price = 0;
		else msg.price = (nextBall * PRICE_INCREASE_PER_EXTRABALL);

		return msg;
	}

	// Checks if any of the cards in play have a winning pattern and updates the credits accordingly
	// @param msg: Message object containing GameState information. In this case, the required information is the cards in play and the user's credits
	// @return Message object containing the updated user credits. Additionally, a message and a code are returned.
	// @note Possible message codes:

	// Success Codes (200-299):
	//	200 (OK) - Everything worked correctly;
	//	202 (Accepted, but not acted upon) - Not all balls were drawn so there shouldn't be a need to check for patterns;

	// Client Error Codes (400-499):
	//	400 (Bad Request) - The message had invalid information (No cards in play);

	// Server Error Codes (500-599):
	//	500 (Internal Server Error)


	BingoProject_API Message check_cards(Message msg)
	{
		// Clear the message string and set the code to 500 (Internal Server Error) in case something goes wrong
		msg.message = "";
		msg.code = 500;
		// Check if there are cards in play
		if (msg.cards.empty()) {
			msg.code = 400; // Bad Request
			msg.message = "No cards in play.";
			return msg;
		}
		// Check if all balls were drawn
		if (msg.drawn_balls.size() < N_BALLS) {
			msg.code = 202; // Accepted, but not acted upon
			msg.message = "Not all balls were drawn.";
			return msg;
		}
		// Check if any of the cards in play have a winning pattern and update the credits accordingly
		check_patterns(msg);
		return msg;
	}
}