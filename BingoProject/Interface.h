#include "Message.h"

#define DLL_EXPORT __declspec(dllexport)

extern "C++" {
	DLL_EXPORT Message generate_cards(Message msg);
	DLL_EXPORT Message draw_ball(Message msg);
	DLL_EXPORT Message check_cards(Message msg);
}