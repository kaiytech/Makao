#ifndef GAME
#define GAME

#include "Player.h"
#include "Card.h"
#include <vector>
#include <string>
#include <algorithm>

enum GAME_TYPE {
	GAME_NONE,
	GAME_LOBBY,
	GAME_GAME
};

class Game {
public:
	Game(int id);
	~Game();

	int GetId();
	int GetNumberOfPlayers();

	bool MakeLobby();
	bool IsLobby();
	bool SetGameHost(Player* player);
	Player *GetGameHost();

	bool MakeGame();
	void PrepareCards();
	void Shuffle();

	bool IsPlayerInGame(Player* player);
	bool AddPlayer(Player* player);
	bool RemovePlayer(Player* player);
	bool RemovePlayer(int playerid);

	std::string ExecuteMove(std::string datain);

	std::string MsgGetLobbyStatus();
	std::string MsgGetGameStatus(int playerid);

private:
	void AddDeck();
	int iGameHostId;

	int iType;
	int iId;

	std::vector<Player*> vPlayers;
	std::vector<Card*> vCards;

	//Game
	int iTurn;
	int iPlayerTurnId;
};


#endif