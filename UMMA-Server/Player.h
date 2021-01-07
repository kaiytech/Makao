#ifndef  PLAYER
#define PLAYER
#include <vector>
#include "../shared/Card.h"
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

	void UpdateLastSeen();
	long long int GetLastSeen();

private:
	int iId;
	long long int ilastSeen;
	std::vector<Card*> vCards;
};
#endif // ! PLAYER
