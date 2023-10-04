#pragma once
#include "Number.h"

class Card
{
public:
	Card();
	vector<vector<Number>> getNumbers() { return numbers; }
	void cashOut() { cashed_out = true; }
	bool isCashedOut() { return cashed_out; }
	void markNumber(unsigned row, unsigned col) { numbers[row][col].mark(); }
private:
	vector<vector<Number>> numbers;
	bool cashed_out = false;
};