#pragma once
#include "consts.h"

// Draws a bingo ball and updates the cards and the drawn balls
void get_ball(Message& msg);

// Checks if any of the cards have a winning pattern and updates the credits
void check_patterns(Message& msg);