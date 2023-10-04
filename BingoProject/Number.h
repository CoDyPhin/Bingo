#pragma once

#include "consts.h"

class Number
{
public:
	Number(unsigned value) { this->value = value; }
	void mark() { is_marked = true; }
	unsigned getValue() { return value; }
	bool isMarked() { return is_marked; }
private:
	unsigned value;
	bool is_marked = false;
};