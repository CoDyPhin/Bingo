#pragma once
#include "Number.h"

class Card
{
public:
	Card();
	~Card();
	Number[] getNumbers() { return numbers; };
	void cashOut() { cashed_out = true; }
	bool isCashedOut() { return cashed_out; }
private:
	Number numbers[N_ROWS][N_COLS];
	bool cashed_out = false;
};