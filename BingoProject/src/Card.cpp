#include "consts.h"

// Number class

Number::Number(unsigned const v) : value(v)
{
	marked = false;
}

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
	cashed_out.resize(N_PATTERNS);
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

void Card::cashOut(unsigned pattern)
{
	cashed_out[pattern-1] = true;
}

bool Card::isCashedOut(unsigned pattern) const
{
	return cashed_out[pattern-1];
}

bool Card::isCashedOut() const
{
	return std::all_of(cashed_out.begin(), cashed_out.end(), [](bool const b) {return b; });
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