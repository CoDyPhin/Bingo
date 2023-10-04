#include <vector>
#include "Message.h"


typedef Message (*generateCards(Message msg));

//typedef Message (*startGame(Message msg));

typedef Message (*drawBall(Message msg));

typedef Message (*checkCard(Message msg));

