// Binary.cpp : Defines the entry point for the application.
//

#include "Binary.h"
#include "shared.h"
#include "dlfcn.h"

using namespace std;

int main()
{
	void* handle = dlopen("libBingoProject.so", RTLD_LAZY);
	if (!handle)
	{
		cout << "Cannot open library: " << dlerror() << '\n';
		return 1;
	}

	using generate_cards_t = Message(*)(Message);
	using draw_ball_t = Message(*)(Message);
	using check_cards_t = Message(*)(Message);

	auto generate_cards = (generate_cards_t)dlsym(handle, "generate_cards");
	auto draw_ball = (draw_ball_t)dlsym(handle, "draw_ball");
	auto check_cards = (check_cards_t)dlsym(handle, "check_cards");

	const char* dlsym_error = dlerror();

	if (dlsym_error)
	{
		cout << "Cannot load symbol create: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	Message msg;
	msg.user_credits = 100;
	msg.num_cards = 1;
	msg = generate_cards(msg);

	for (int i = 0; i < N_BALLS; i++)
	{
		msg = draw_ball(msg);
	}

	msg = check_cards(msg);

	dlclose(handle);

	return 0;
}
