#include "SessionHandler.h"
#include "Game.h"
#include <string>
#include <stdexcept>
#include <chrono>

static SessionHandler* __g_sh = 0;

// Creates an instance of a session handler if needed, and returns it
SessionHandler* GetSessionHandler() {
	if (!__g_sh)
		__g_sh = new SessionHandler();
	return __g_sh;
}


// Constructor
SessionHandler::SessionHandler() {
	iNumOfGames = 0;
	iNumOfPlayers = 0;
}

// Creates a game and returns its ID
int SessionHandler::CreateGame() {
	iNumOfGames++;
	
	Msg("[S] Creating a new game with an ID" << iNumOfGames);
	Game* g = new Game(iNumOfGames);

	vGames.push_back(g);
	Success("[S] New game #" << iNumOfGames << " created!");
	return iNumOfGames;
}

// Creates a new player and returns their new ID
int SessionHandler::CreatePlayer() {
	iNumOfPlayers++;
	Msg("[S] Creating a new player with an ID" << iNumOfPlayers);
	Player* p = new Player(iNumOfPlayers);
	vPlayers.push_back(p);
	p->UpdateLastSeen();
	Success("[S] New player #" << iNumOfPlayers << " created!");

	return iNumOfPlayers;
}

// kicks the player out of any lobby or game they're in.
// returns true on success
bool SessionHandler::KickPlayer(int pid) {
	for (int i = 0; i < vPlayers.size(); i++) {
		for (int g = 0; g < vGames.size(); g++) {
			if (vGames[g]->IsPlayerInGame(vPlayers[i])) return vGames[g]->RemovePlayer(pid);
		}
	}
	return false;
}

// returns the game given player is in
// returns NULL on failure
Game* SessionHandler::IsPlayerInGame(Player* player) {
	if (!player) return NULL;

	int pid = player->GetId();

	for (int g = 0; g < vGames.size(); g++) {
		if (vGames[g]->IsPlayerInGame(player)) return vGames[g];
	}

	return NULL;
}



// Returns the game by ID. Returns NULL on failure.
Game* SessionHandler::GetGame(int gameId) {
	for (int i = 0; i < vGames.size(); i++) {
		if (vGames[i]->GetId() == gameId) return vGames[i];
	}
	return NULL;
}

// Returns the player by ID. Returns NULL on failure.
Player* SessionHandler::GetPlayer(int playerId) {
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == playerId) return  vPlayers[i];
	}
	return NULL;
}

std::string SessionHandler::GetGameList() {
	std::string r = "lobbylist|";
	for (int g = 0; g < vGames.size(); g++) {
		if (vGames[g]->GetNumberOfPlayers() == 0) continue;
		r.append(std::to_string(vGames[g]->GetId()));
		r.append("+");
		r.append(std::to_string(vGames[g]->GetNumberOfPlayers()));
		r.append("|");
	}
	r.append("-");
	return r;
}

bool SessionHandler::ParseAndExecuteJoinLobby(std::string datain) {
	std::string tocut = datain;
	int sap = tocut.find("|");
	std::string playerid = tocut.substr(0, sap);
	std::string roomid = tocut.substr(sap + 1, tocut.length());
	if (stoi(roomid) == -1) return false;
	return JoinLoby(stoi(playerid), stoi(roomid));
}

bool SessionHandler::JoinLoby(int playerid, int lobbyid) {
	Msg("[S] Attempting to assign player #" << playerid << " to game #" << lobbyid);
	Player *player = GetPlayer(playerid);
	if (!player) {
		Error("[S] Invalid player!");
		return false;
	}
	player->UpdateLastSeen();
	if (IsPlayerInGame(player)) {
		Warn("[S] Player already in other game! Kicking.");
		KickPlayer(playerid);
	}

	Game* game = GetGame(lobbyid);
	if (!game) {
		Error("[S] Invalid game!");
		return false;
	}
	return game->AddPlayer(player);
}

bool SessionHandler::ParseAndExecuteBeginGame(std::string datain) {
	Msg("[S] Attempting to promote lobby to a game. Player #" << datain);
	std::string tocut = datain;
	int playerid;
	try { playerid = stoi(tocut); }
	catch (const std::invalid_argument& ia) { return false; }
	catch (const std::out_of_range& oor) { return false; }
	catch (const std::exception& e) { return false; }

	Player* p = GetPlayer(playerid);
	if (!p) {
		Error("[S] Invalid player!");
		return false;
	}

	p->UpdateLastSeen();

	Game* game = NULL;

	for (int g = 0; g < vGames.size(); g++) {
		if (vGames[g]->IsPlayerInGame(p) && (vGames[g]->GetGameHost() == p)) game = vGames[g];
	}

	if (!game) {
		Warn("[S] Player " << playerid << "can't begin game they're not host of! Aborting.");
		return false;
	}

	if (game->GetNumberOfPlayers() <= 1) {
		Warn("[S] Player " << playerid << " can't begin the game because there is too few players.");
		return false;
	}

	return game->MakeGame();

}

std::string SessionHandler::ParseAndExecuteMove(std::string datain) {
	std::string s = datain;
	int sap = s.find("|");
	s = s.substr(sap + 1, s.length());

	sap = s.find("|");
	std::string stringid = s.substr(0, sap);
	int playerid;
	try { playerid = stoi(stringid); }
	catch (const std::invalid_argument& ia) { return "AFS"; }
	catch (const std::out_of_range& oor) { return "AFS"; }
	catch (const std::exception& e) { return "AFS"; }

	Player* pPlayer = GetPlayer(playerid);
	if (!pPlayer) return "AFS";
	pPlayer->UpdateLastSeen();
	Game * pGame = IsPlayerInGame(pPlayer);
	if (!pGame) return "AFS";

	return pGame->ExecuteMove(datain);
}

void SessionHandler::KickAFKs() {
	for (int i = 0; i < vPlayers.size(); i++) {
		std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds> (std::chrono::system_clock::now().time_since_epoch());
		if (s.count() - vPlayers[i]->GetLastSeen() > 30) {
			if (IsPlayerInGame(vPlayers[i])) {
				KickPlayer(vPlayers[i]->GetId());
				Msg("[S] Player #" << vPlayers[i]->GetId() << " kicked. Reason: connection lost");
			}
		}
	}
}

// Destructor
SessionHandler::~SessionHandler() {

}