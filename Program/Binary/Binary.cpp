// Binary.cpp : Defines the entry point for the application.
//

#include "Binary.h"
#include "shared.h"
#include "dlfcn.h"

// Game Loop State machine states
enum state
{
	ST_EXIT, // Exit the game
	ST_GEN_CARDS, // Prompt the user for the number of cards to generate
	ST_START_GAME, // Prompt the user to start the game or re-generate cards
	ST_DRAW_BALL, // Draw a ball
	ST_CHECK_CARDS, // Check the cards for a win
	ST_END_GAME // Prompt the user to exit, play again or buy an extra ball
};


using namespace std;


// Load the user's credits
// In the future, this function should be replaced with a call to the server
unsigned load_user_credits()
{
	return 10000;
}

// An auxiliary function to check for dlerrors (linking errors) and print them to the console
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

// Auxiliary function to prompt the user for input
// @param min_input: The minimum input value
// @param max_input: The maximum input value
// @param prompt: The prompt to display to the user
// @return: The user's sanitized input
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

// The Game's main function
int main()
{
	// Start by loading the Bingo library
	void* handle = dlopen("libBingoProject.so", RTLD_LAZY);
	if (!handle)
	{
		cout << "Cannot open library: " << dlerror() << '\n';
		return 1;
	}

	using generate_cards_t = Message(*)(Message);
	using draw_ball_t = Message(*)(Message);
	using check_cards_t = Message(*)(Message);

	// Load the generate_cards method
	auto const generate_cards = reinterpret_cast<generate_cards_t>(dlsym(handle, "_Z14generate_cards7Message"));
	if (check_dl_error()) {
		dlclose(handle);
		return 1;
	}

	// Load the draw_ball method
	auto const draw_ball = reinterpret_cast<draw_ball_t>(dlsym(handle, "_Z9draw_ball7Message"));
	if (check_dl_error()){
		dlclose(handle);
		return 1;
	}

	// Load the check_cards method
	auto const check_cards = reinterpret_cast<check_cards_t>(dlsym(handle, "_Z11check_cards7Message"));
	if (check_dl_error()) {
		dlclose(handle);
		return 1;
	}


	// Start the game
	unsigned credits = load_user_credits();
	unsigned state = 1;
	unsigned input;
	cout << "Welcome to Bingo!" << endl;
	Message msg;

	// The game's main loop
	while (true)
	{
		switch (state) // State Machine

		//	Any State ==================|===============================================|
		//								|												|	
		//				(On Error Code)	|  												|
		//								v     	     (Input 0)							|
		//	Entry ================> ST_GEN_CARDS =================> ST_EXIT => Exit		|
		//							    | ^								^				|
		//				  (Input N > 0) | |	(Input 2)					|				|
		//								v |								| (Input 0)		|
		//							  ST_START_GAME ====================|				|
		//								|								|				|
		//					|=======|	| (Input 1)						|				| (Input 1)
		//					|		v	v								|				|
		//					|====== ST_DRAW_BALL <======|				|				| 
		//								| ^				|				|				| 
		//		  (DrawnBalls>=N_BALLS) | |	(Code 202)	|				| (Input 0)		|	
		//								v |				| (Input 2)		|				|
		//							ST_CHECK_CARDS		|				|				|
		//								|				|				|				|
		//								|				|				|				|
		//								v				|				|				|
		//							ST_END_GAME ========|===============|				|
		//								|												|
		//								|												|
		//								|===============================================|

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
				string prompt = "Start game?\n[0] Exit\n[1] Yes";
				// If there is a price, add it to the prompt
				if(msg.price > 0) prompt += std::string(" (Price: ").append(to_string(msg.price)).append(")");
				prompt += "\n[2] Re-generate cards";
				input = prompt_user(0, 2, prompt);
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
				if (msg.code == 202) { // For some reason, the number of balls to draw is not the same as the server's
					state = ST_DRAW_BALL; 
					continue;
				}
				if(msg.code > 299 || msg.code < 200)
				{
					state = ST_GEN_CARDS;
					continue;
				}
				cout << "Current credits: " << msg.user_credits << endl;
				credits = msg.user_credits;
				state = ST_END_GAME;
				break;
			}
			case ST_END_GAME:
			{
				string priceStr;
				// Add the price of the extra ball to the prompt, or if the max number has been reached
				if (msg.price > 0) priceStr = std::string(" (Price: ").append(to_string(msg.price)).append(")");
				else priceStr = " (Max Number Reached)";
				input = prompt_user(0, 2, std::string("[0] Exit\n[1] Play Again\n[2] Buy Extra Ball").append(priceStr));
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