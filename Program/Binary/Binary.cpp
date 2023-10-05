// Binary.cpp : Defines the entry point for the application.
//

#include "Binary.h"
#include "shared.h"
#include "dlfcn.h"

using namespace std;

unsigned load_user_credits()
{
	// Load credits from where they are stored
	return 100;
}


unsigned prompt_user(unsigned const min_input, unsigned const max_input, std::string const& prompt)
{
	unsigned input;
	while (true)
	{
		cout << prompt << endl;
		cin >> input;
		if (!(input < min_input || input > max_input)) break;
		cout << "Invalid input. Please try again." << endl;
	}
	return input;
}

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

	auto const generate_cards = reinterpret_cast<generate_cards_t>(dlsym(handle, "_Z14generate_cards7Message"));
	const char* dlsym_error = dlerror();

	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	auto const draw_ball = reinterpret_cast<draw_ball_t>(dlsym(handle, "_Z9draw_ball7Message"));

	dlsym_error = dlerror();
	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	auto const check_cards = reinterpret_cast<check_cards_t>(dlsym(handle, "_Z11check_cards7Message"));

	dlsym_error = dlerror();
	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		dlclose(handle);
		return 1;
	}

	unsigned credits = load_user_credits();
	unsigned state = 0;
	unsigned input;
	cout << "Welcome to Bingo!" << endl;
	Message msg = {};
	while (true)
	{
		switch (state)
		{
			case 0:
			{
				input = prompt_user(0, MAX_CARDS, "How many cards do you wish to generate? (0 to Exit)");
				if (input == 0)
				{
					cout << "Exiting..." << endl;
					return 0;
				}
				msg = {};
				msg.num_cards = input;
				msg.user_credits = credits;
				msg = generate_cards(msg);
				cout << msg.message << endl;
				if (msg.code != 200) continue;
				for (const Card& c : msg.cards)
				{
					cout << c << endl << endl;
				}
				state = 1;
				break;
			}
			case 1:
			{
				input = prompt_user(0, 2, "Start game? \n[0] Exit\n[1] Yes\n[2] Re-generate cards");
				if(input == 0)
				{
					cout << "Exiting..." << endl;
					return 0;
				}
				if (input == 1)
				{
					state = 2;
					break;
				}
				if (input == 2)
				{
					state = 0;
					break;
				}
				break;
			}
			case 2:
			{
				for (int i = 0; i < N_BALLS; i++)
				{
					msg = draw_ball(msg);
					cout << msg.message << endl;
					if(msg.code != 200)
					{
						state = 0;
						continue;
					}
					int cnt = 1;
					for (const Card& c : msg.cards) {
						cout << "Card " << cnt << ":" << endl;
						cout << c << endl << endl;
						cnt++;
					}
				}
				state = 3;
				break;
			}
			case 3:
			{
				msg = check_cards(msg);
				cout << msg.code << endl;
				cout << msg.message << endl;
				cout << "Current credits: " << msg.user_credits << endl;
				dlclose(handle);
				return 0;
			}
		default:
			cout << "Invalid state. Exiting..." << endl;
			return 1;
		}
	}

	

	return 0;
}