#include "consts.h"

// Number class

Number::Number(unsigned const v) : value(v)
{
	marked = false;
}

/*Number::Number() : value(0), marked(false)
{
}

Number::Number(const Number& n) : value(n.value), marked(n.marked)
{
}*/

unsigned Number::getValue() const
{
	return value;
}

bool Number::isMarked() const
{
	return marked;
}

void Number::mark()
{
	marked = true;
}


// Card class

Card::Card()
{
	//numbers = std::vector(N_ROWS, std::vector<Number>(N_COLS));
	std::vector<unsigned> range(MAX_BALL_NUM - MIN_BALL_NUM + 1);
	std::iota(range.begin(), range.end(), MIN_BALL_NUM);
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(range.begin(), range.end(), g);
	range.resize(N_ROWS * N_COLS);
	for (unsigned i = 0; i < N_ROWS; i++) {
		std::vector<Number> row;
		for (unsigned j = 0; j < N_COLS; j++) {
			row.emplace_back(range[i * N_COLS + j]);
		}
		numbers.push_back(row);
	}
}

std::vector<std::vector<Number>> Card::getNumbers() const
{
	return numbers;
}

void Card::cashOut() {
	cashed_out = true;
}

bool Card::isCashedOut() const {
	return cashed_out;
}

void Card::markNumber(unsigned const row, unsigned const col) {
	numbers[row][col].mark();
}

std::ostream& operator<<(std::ostream& os, const Card& c)
{
	for (auto& row : c.getNumbers()) {
		for (auto& num : row) {
			os << std::setw(3) << num.getValue() << (num.isMarked() ? " (X)" : "    ") << "   ";
		}
		os << '\n';
	}
	return os;
}