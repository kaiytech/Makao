#ifndef SESSION_HANDLER
#define SESSION_HANDLER

#include <vector>
#include "Game.h"
#include "Player.h"

class SessionHandler {
public:
	SessionHandler();
	~SessionHandler();

	int CreateGame();

	int CreatePlayer();
	bool KickPlayer(int pid);
	Game* IsPlayerInGame(Player* player);

	Game* GetGame(int gameId);
	Player* GetPlayer(int playerId);

	bool ParseAndExecuteJoinLobby(std::string datain);
	bool JoinLoby(int playerid, int lobbyid);

	std::string GetGameList();

private:
	std::vector<Game*> vGames;
	int iNumOfGames;

	std::vector<Player*> vPlayers;
	int iNumOfPlayers;
};

SessionHandler* GetSessionHandler();

#endif