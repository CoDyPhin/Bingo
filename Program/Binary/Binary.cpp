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

	auto generate_cards = (generate_cards_t)dlsym(handle, "_Z14generate_cards7Message");
	const char* dlsym_error = dlerror();

	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	auto draw_ball = (draw_ball_t)dlsym(handle, "_Z9draw_ball7Message");

	dlsym_error = dlerror();
	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	auto check_cards = (check_cards_t)dlsym(handle, "_Z11check_cards7Message");

	dlsym_error = dlerror();
	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	Message msg;
	msg.user_credits = 100;
	msg.num_cards = 1;
	msg = generate_cards(msg);

	cout << msg.message << endl;
	//cout << msg.code << endl;

	for (int i = 0; i < N_BALLS; i++)
	{
		msg = draw_ball(msg);
		cout << msg.message << endl;
		for (auto row: msg.cards[0].getNumbers())
		{
			for (auto num : row)
			{
				cout << " " << num.getValue() << (num.isMarked() ? "(X) " : "    ") << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	msg = check_cards(msg);
	cout << msg.code << endl;
	cout << msg.message << endl;
	cout << "Current credits: " << msg.user_credits << endl;

	dlclose(handle);

	return 0;
}
