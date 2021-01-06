#ifndef CARD
#define CARD

#include <string>

enum CARD_TYPE {
	TYPE_A,
	TYPE_2,
	TYPE_3,
	TYPE_4,
	TYPE_5,
	TYPE_6,
	TYPE_7,
	TYPE_8,
	TYPE_9,
	TYPE_10,
	TYPE_J,
	TYPE_Q,
	TYPE_K,
	TYPE_LENGTH //this has to be last!!
};

enum CARD_SUIT {
	SUIT_CLUB,
	SUIT_HEART,
	SUIT_SPADE,
	SUIT_DIAMOND,
	SUIT_LENGTH // this has to be last!!
};

class Card {
public:
	Card(CARD_TYPE type, CARD_SUIT suit);

	CARD_TYPE GetType();
	CARD_SUIT GetSuit();

	std::string GetString();

	bool CanBePutOnTop();

private:
	CARD_TYPE eType;
	CARD_SUIT eSuit;
};

#endif