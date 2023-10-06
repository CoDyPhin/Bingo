#pragma once

#include "utils.h"

extern "C++" {
	BingoProject_API Message generate_cards(Message msg);
	BingoProject_API Message draw_ball(Message msg);
	BingoProject_API Message check_cards(Message msg);
}