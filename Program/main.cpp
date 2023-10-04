#include <dlfcn.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>

using namespace std;

int main()
{
	void* handle;

	handle = dlopen("./libBingoProject.so", RTLD_LAZY);
	if (!handle)
	{
		cout << "Error loading library: " << dlerror() << endl;
		return -1;
	}

	using generate_cards_t = Message(*)(Message);
	using draw_ball_t = Message(*)(Message);
	using check_cards_t = Message(*)(Message);

	generate_cards_t generate_cards = (generate_cards_t)dlsym(handle, "generate_cards");

	draw_ball_t draw_ball = (draw_ball_t)dlsym(handle, "draw_ball");

	check_cards_t check_cards = (check_cards_t)dlsym(handle, "check_cards");


	const char* dlsym_error = dlerror();

	if (dlsym_error) {

		std::cerr << "Cannot load symbol 'algorithm': " << dlsym_error << '\n';

		dlclose(handle);

		return 1;

	}


	Message msg;

	msg.user_credits = 100;

	msg.num_cards = 1;


	msg = generate_cards(msg);

	for (int i = 0; i < N_BALLS + N_MAX_EXTRABALLS; i++)

		msg = draw_ball(msg);

	msg = check_cards(msg);

	dlclose(handle);

	return 0;
}

	/*Message message;
	message.num_cards = 1;
	message.user_credits = 100;

	Message response = generate_cards(message);
	if (message.code != 200)
	{
		return -1;
	}
	cout << "Message: " << message.message << endl;

	for (int i = 0; i < message.cards.size(); i++)
	{
		cout << "Card " << i << endl;
		vector<vector<Number>> numbers = message.cards[i].getNumbers();

		for (auto row : numbers)
		{
			for (auto number : row)
			{
				cout << std::setw(2) << number.getValue() << ' ';
				if (number.isMarked())
				{
					cout << "(X) " << std::setw(2) << number.getValue() << ' ';
				}
				else
				{
					cout << "    " << std::setw(2) << number.getValue() << ' ';
				}
			}
			cout << '\n';
		}
	}

	return 0;
}*/
