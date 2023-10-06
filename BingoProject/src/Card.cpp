#include "consts.h"

// Number class

// The Number class represents an individual number in a Bingo card.
// It has a value and a boolean flag to indicate whether it has been marked (the respective ball value has been drawn).

// Number Constructor: Constructs a new Number object with the given value and unmarked.
// @params v: The value of the number.
// @return A new Number object.

Number::Number(unsigned const v) : value(v)
{
	marked = false;
}


// getValue Method: Returns the value of the number.
// @return The value of the number.

unsigned Number::getValue() const
{
	return value;
}

// isMarked Method: Returns whether the number has been marked.
// @return True if the number has been marked, false otherwise.

bool Number::isMarked() const
{
	return marked;
}

// mark Method: Marks the number.

void Number::mark()
{
	marked = true;
}


// Card class

// The Card class represents a Bingo card.

// Card Constructor: Uses the shuffle algorithm and the Mersenne-Twister pseudo-random generator to generate a Bingo card.
// @return A new Card object.

Card::Card()
{
	// Start the card with all patterns not cashed out.
	cashed_out.resize(N_PATTERNS);

	// Generate a vector with all possible numbers.
	std::vector<unsigned> range(MAX_BALL_NUM - MIN_BALL_NUM + 1);
	std::iota(range.begin(), range.end(), MIN_BALL_NUM);

	// Use the Mersenne-Twister pseudo-random generator to shuffle the vector.
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(range.begin(), range.end(), g);

	// Select the first N_ROWS * N_COLS numbers in the shuffled vector and add them to the numbers vector of the card.
	range.resize(N_ROWS * N_COLS);
	for (unsigned i = 0; i < N_ROWS; i++) {
		std::vector<Number> row;
		for (unsigned j = 0; j < N_COLS; j++) {
			row.emplace_back(range[i * N_COLS + j]);
		}
		numbers.push_back(row);
	}
}

// getNumbers Method: Returns the numbers vector in the card.
// @return The numbers vector in the card.

std::vector<std::vector<Number>> Card::getNumbers() const
{
	return numbers;
}

// cashOut Method: Marks the card as cashed out for a given pattern.
// @params pattern: The pattern that is being cashed out.

void Card::cashOut(unsigned pattern)
{
	cashed_out[pattern-1] = true;
}

// isCashedOut Method: Returns whether the card has been cashed out for a given pattern.
// @params pattern: The pattern that is being checked.

bool Card::isCashedOut(unsigned pattern) const
{
	return cashed_out[pattern-1];
}

// isCashedOut Method: Returns whether the card has been cashed out for all patterns.

bool Card::isCashedOut() const
{
	return std::all_of(cashed_out.begin(), cashed_out.end(), [](bool const b) {return b; });
}

// markNumber Method: Marks a specific number in the card.
// @params row: The row index of the number in the numbers vector.
// @params col: The column index of the number in the numbers vector.

void Card::markNumber(unsigned const row, unsigned const col) {
	if(row < N_ROWS && col < N_COLS) numbers[row][col].mark();
}

// operator<< Method: Overloads the << operator to print the card.
// @params os: The output stream.
// @params c: The card to be printed.
// @return The output stream.

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