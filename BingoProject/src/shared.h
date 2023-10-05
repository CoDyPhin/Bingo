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
	Number(int v) : value(v) {}
	bool marked = false;
	int value;
	int getValue() const { return value; }
	bool isMarked() const { return marked; }
	void mark() { marked = true; }
};


//Card class declarations

class Card
{
public:
	Card();
	std::vector<std::vector<Number>> getNumbers();
	void cashOut() { cashed_out = true; }
	bool isCashedOut() { return cashed_out; }
	void markNumber(unsigned row, unsigned col) { numbers[row][col].mark(); }
private:
	std::vector<std::vector<Number>> numbers;
	bool cashed_out = false;
};


// Message Structure

struct Message
{
	int code = 0;
	int user_credits = 0;
	int num_cards = 0;
	std::string message;
	std::vector<Card> cards;
	std::unordered_set<unsigned> drawn_balls;
};