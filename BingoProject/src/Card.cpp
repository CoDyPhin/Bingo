#include "consts.h"


Card::Card()
{
	numbers = std::vector<std::vector<Number>>(N_ROWS, std::vector<Number>(N_COLS));
	std::vector<unsigned> range(MAX_BALL_NUM - MIN_BALL_NUM + 1);
	std::iota(range.begin(), range.end(), MIN_BALL_NUM);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(range.begin(), range.end(), g);
	range.resize(N_ROWS * N_COLS);
	for (unsigned i = 0; i < N_ROWS; i++) {
		for (unsigned j = 0; j < N_COLS; j++) {
			numbers[i][j] = Number(range[i * N_COLS + j]);
		}
	}
}

std::vector<std::vector<Number>> Card::getNumbers()
{
	return numbers;
}

void Card::cashOut() {
	cashed_out = true;
}

bool Card::isCashedOut() {
	return cashed_out;
}

void Card::markNumber(unsigned row, unsigned col) {
	numbers[row][col].mark();
}