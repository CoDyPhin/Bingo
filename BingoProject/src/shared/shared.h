#pragma once

#include <vector>
#include <unordered_set>
#include <string>

// Game consts

#define MAX_CARDS 1
#define N_BALLS 30
#define N_MAX_EXTRABALLS 0

// Number Structure

struct Number
{
	Number() { value = 0; }
	Number(unsigned v) { value = v; }
	bool marked = false;
	unsigned value;
	unsigned getValue() const { return value; }
	bool isMarked() const { return marked; }
	void mark() { marked = true; }
};


//Card class declarations

class Card
{
public:
	Card();
	std::vector<std::vector<Number>> getNumbers();
	void cashOut();
	bool isCashedOut();
	void markNumber(unsigned row, unsigned col);
private:
	std::vector<std::vector<Number>> numbers;
	bool cashed_out = false;
};


// Message Structure

struct Message
{
	unsigned code = 0;
	unsigned user_credits = 0;
	unsigned num_cards = 0;
	std::string message;
	std::vector<Card> cards;
	std::unordered_set<unsigned> drawn_balls;
};