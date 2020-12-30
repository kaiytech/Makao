#ifndef GAME
#define GAME

#include "Player.h"
#include <vector>
#include <string>

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

	bool IsPlayerInGame(Player* player);
	bool AddPlayer(Player* player);
	bool RemovePlayer(Player* player);
	bool RemovePlayer(int playerid);

	std::string MsgGetLobbyStatus();

private:
	int iGameHostId;

	int iType;
	int iId;

	std::vector<Player*> vPlayers;
};


#endif