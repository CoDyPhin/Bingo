#pragma once

#include <vector>
#include <unordered_set>
#include <string>

// Game consts

#define MAX_CARDS 1 // Maximum number of cards a user can buy in each game
#define N_BALLS 30 // Number of balls drawn in each game

// Number class declarations

// The Number class represents an individual number in a Bingo card.
// It has a value and a boolean flag to indicate whether it has been marked (the respective ball value has been drawn).
class Number
{
public:
	Number(unsigned v);
	unsigned getValue() const;
	bool isMarked() const;
	void mark();
private:
	unsigned const value;
	bool marked;
};


//Card class declarations

// The Card class represents a Bingo card.
// It has a vector of vectors of Numbers, representing the card's numbers, and a vector of booleans, representing which patterns have been cashed out.
class Card
{
public:
	Card();
	std::vector<std::vector<Number>> getNumbers() const;
	void cashOut(unsigned pattern);
	bool isCashedOut(unsigned pattern) const;
	bool isCashedOut() const;
	void markNumber(unsigned row, unsigned col);
	friend std::ostream& operator<<(std::ostream& os, const Card& c);
private:
	std::vector<std::vector<Number>> numbers;
	std::vector<bool> cashed_out;
};


// Message Structure

// The Message structure represents a message with GameState information, transmitted between the GameEngine and the Shared Library.

struct Message
{
	unsigned code = 0; // Response code similar to the HTTP protocol
	unsigned user_credits = 0; // The user's credits
	unsigned num_cards = 0; // The number of cards the user has bought or wishes to buy
	unsigned price = 0; // The price of the next action (buying n cards or drawing the next extra ball)
	std::string message; // An informative message string
	std::vector<Card> cards; // The user's cards
	std::unordered_set<unsigned> drawn_balls; // The balls that have been drawn in the current game
};