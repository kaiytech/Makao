#ifndef  PLAYER
#define PLAYER
#include <vector>
#include "Card.h"
#include "debug.h"

class Player {
public:
	Player(int id);
	int GetId();

	void AddCard(Card* card);
	bool RemoveCard(Card* card);

	bool HasCard(Card* card);
	Card* HasCard(std::string datain);
	Card* GetCard(int index);
	int GetCardAmount();
	std::string GetCards();

private:
	int iId;
	std::vector<Card*> vCards;
};
#endif // ! PLAYER
