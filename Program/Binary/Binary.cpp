// Binary.cpp : Defines the entry point for the application.
//

#include "Binary.h"
#include "shared.h"
#include "dlfcn.h"

enum state
{
	ST_EXIT,
	ST_GEN_CARDS,
	ST_START_GAME,
	ST_DRAW_BALL,
	ST_CHECK_CARDS,
	ST_END_GAME
};


using namespace std;

unsigned load_user_credits()
{
	// Load credits from where they are stored
	return 10000;
}

bool check_dl_error()
{
	const char* dlsym_error = dlerror();
	if (dlsym_error)
	{
		cout << "Cannot load symbol: " << dlsym_error << '\n';
		return true;
	}
	return false;
}


unsigned prompt_user(unsigned const min_input, unsigned const max_input, std::string const& prompt)
{
	unsigned input;
	while (true)
	{
		cout << prompt << endl << "Input: ";
		if(cin >> input)
		{
			if (!(input < min_input || input > max_input)) break;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
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
	if (check_dl_error()) {
		dlclose(handle);
		return 1;
	}

	auto const draw_ball = reinterpret_cast<draw_ball_t>(dlsym(handle, "_Z9draw_ball7Message"));
	if (check_dl_error()){
		dlclose(handle);
		return 1;
	}

	auto const check_cards = reinterpret_cast<check_cards_t>(dlsym(handle, "_Z11check_cards7Message"));
	if (check_dl_error()) {
		dlclose(handle);
		return 1;
	}

	unsigned credits = load_user_credits();
	unsigned state = 1;
	unsigned input;
	cout << "Welcome to Bingo!" << endl;
	Message msg;
	while (true)
	{
		switch (state)
		{
			case ST_EXIT:
			{
				cout << "Exiting..." << endl;
				dlclose(handle);
				return 0;
			}
			case ST_GEN_CARDS:
			{
				input = prompt_user(0, MAX_CARDS, "How many cards do you wish to generate? (0 to Exit)");
				if (input == 0)
				{
					state = ST_EXIT;
					continue;
				}
				msg = {};
				msg.num_cards = input;
				msg.user_credits = credits;
				msg = generate_cards(msg);
				cout << msg.message << endl;
				if (msg.code > 299 || msg.code < 200) continue;
				for (const Card& c : msg.cards) { cout << c << endl << endl; }
				state = ST_START_GAME;
				break;
			}
			case ST_START_GAME:
			{
				input = prompt_user(0, 2, "Start game? \n[0] Exit\n[1] Yes\n[2] Re-generate cards");
				if(input == 0)
				{
					state = ST_EXIT;
					continue;
				}
				if (input == 1)
				{
					state = ST_DRAW_BALL;
					continue;
				}
				if (input == 2)
				{
					state = ST_GEN_CARDS;
					continue;
				}
				break;
			}
			case ST_DRAW_BALL:
			{
				msg = draw_ball(msg);
				cout << msg.message << endl;
				if(msg.code > 299 || msg.code < 200)
				{
					state = ST_GEN_CARDS;
					continue;
				}
				int cnt = 1;
				for (const Card& c : msg.cards) {
					cout << "Card " << cnt << ":" << endl << c << endl << endl;
					cnt++;
				}
				if (msg.drawn_balls.size() >= N_BALLS) {
					if(msg.code == 202) state = ST_END_GAME;
					else state = ST_CHECK_CARDS;
				}
				break;
			}
			case ST_CHECK_CARDS:
			{
				msg = check_cards(msg);
				cout << msg.message << endl;
				if(msg.code > 299 || msg.code < 200)
				{
					if (msg.code == 406) state = ST_DRAW_BALL; // For some reason, the number of balls to draw is not the same as the server's
					else state = ST_GEN_CARDS;
					continue;
				}
				cout << "Current credits: " << msg.user_credits << endl;
				credits = msg.user_credits;
				state = ST_END_GAME;
				break;
			}
			case ST_END_GAME:
			{
				input = prompt_user(0, 2, "[0] Exit\n[1] Play Again\n[2] Buy Extra Ball");
				if(input == 0)
				{
					state = ST_EXIT;
					continue;
				}
				if(input == 1)
				{
					state = ST_GEN_CARDS;
					continue;
				}
				if(input == 2)
				{
					state = ST_DRAW_BALL;
					continue;
				}
				break;
			}
		default:
			cout << "Invalid state. Exiting..." << endl;
			dlclose(handle);
			return 1;
		}
	}

	return 1;
}