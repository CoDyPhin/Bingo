#pragma once

#include "Message.h"

extern "C++" {
	BingoProject_EXPORTS Message generate_cards(Message msg);
	BingoProject_EXPORTS Message draw_ball(Message msg);
	BingoProject_EXPORTS Message check_cards(Message msg);
}