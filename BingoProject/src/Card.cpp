#include "Card.h"

Card::Card()
{
	numbers = vector<vector<Number>>(N_ROWS, vector<Number>(N_COLS));
	vector<unsigned> range = vector(MAX_BALL_NUM - MIN_BALL_NUM + 1);
	iota(range.begin(), range.end(), MIN_BALL_NUM);
	random_device rd;
	mt19937 g(rd());
	shuffle(range.begin(), range.end(), g);
	range.resize(N_ROWS * N_COLS);
	for (unsigned i = 0; i < N_ROWS; i++) {
		for (unsigned j = 0; j < N_COLS; j++) {
			numbers[i][j] = Number(range[i * N_COLS + j]);
		}
	}
}


/*ostream& operator<<(ostream& os, const Card& card) {

	for (auto row : card.numbers) {
		for (auto number : row) {
			os << std::setw(2) << number.getValue() << ' ';
			if (number.isMarked()) {
				os << "(X) " << std::setw(2) << number.getValue() << ' ';
			}
			else {
				os << "    " << std::setw(2) << number.getValue() << ' ';
			}
		}
		os << '\n';
	}
	return os;
}*/