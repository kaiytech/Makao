#ifndef GAME
#define GAME

#include "../shared/Card.h"
#include <vector>

#define game GetGame()

class Game {
public:
	Game();
	bool IsInLobby();
	bool IsInGame();
	bool IsInLobbyList();
	bool IsInGameOver();
	bool IsInCardPlanning();
	bool IsMyTurn();
	int GetCachedNumber();

	void SetInLobby(bool state);
	void SetInGame(bool state);
	void SetInLobbyList(bool state);
	void SetInGameOver(bool state);
	void SetInCardPlanning(bool state);
	void SetMyTurn(bool state);
	void SetCachedNumber(int num);

	void AssignId(int id);
	bool HasId();
	int GetId();

	void ClearCards();
	void AddCard(Card* card);
	Card* GetCard(int id);

	void SetPlannedCard(Card* card, int num);
	Card* GetPlannedCard();
	void UnsetPlannedCard();

private:
	bool bInLobby;
	bool bInGame;
	bool bInLobbyList;
	bool bInGameOver;
	bool bInCardPlanning;
	bool bMyTurn;
	int iId;

	int iCachedNumber;

	bool bTurn;

	std::vector<Card*> vCards;
	Card* pCardPlanned;
	
};

Game* GetGame();

#endif