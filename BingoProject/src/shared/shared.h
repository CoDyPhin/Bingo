#pragma once

#include <vector>
#include <unordered_set>
#include <string>

// Game consts

#define MAX_CARDS 1
#define N_BALLS 30
#define N_MAX_EXTRABALLS 0

// Number class declarations

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

class Card
{
public:
	Card();
	std::vector<std::vector<Number>> getNumbers() const;
	void cashOut();
	bool isCashedOut() const;
	void markNumber(unsigned row, unsigned col);
	friend std::ostream& operator<<(std::ostream& os, const Card& c);
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