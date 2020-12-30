#include "Game.h"

Game::Game(int id) {
	iId = id;
	iType = GAME_NONE;
	iGameHostId = -1;
}

int Game::GetId() {
	return iId;
}

int Game::GetNumberOfPlayers() {
	int n = 0;
	for (int i = 0; i < vPlayers.size(); i++) {
		n++;
	}
	return n;
}

bool Game::MakeLobby() {
	iType = GAME_LOBBY;
	return true; //?
}

bool Game::IsLobby() {
	if (iType == GAME_LOBBY) return true;
	else return false;
}

// returns true on success
bool Game::SetGameHost(Player* player) {
	if (!player) return false;
	iGameHostId = player->GetId();
	return true;
}

// returns Player* if game host is set
// returns NULL otherwise
Player* Game::GetGameHost() {
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == iGameHostId) return vPlayers[i];
	}
	return NULL;
}

bool Game::IsPlayerInGame(Player* player) {
	if (!player) return false;

	int pid = player->GetId();

	// check if the player is in the game
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == pid) return true; // return true if so
	}
	return false; // return false if not
}

// returns true on success or false on failure
bool Game::AddPlayer(Player *player) {
	if (!player) return false;

	int pid = player->GetId();

	// check if the player isn't already in the game
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == pid) return false; // and return false if is
	}

	vPlayers.push_back(player);
	return true; // success. ...presumably...
}

//todo
bool Game::RemovePlayer(Player *player) {
	return true;
}

bool Game::RemovePlayer(int playerid) {
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == playerid) vPlayers.erase(vPlayers.begin() + i);
	}
	return true; //hm.
}

// outputs the lobby status in the following syntax:
// lobbystatus|...|...|*lobbyid-
// ... - players in lobby
// 
// eg. lobbystatus|4|7|12|*4-
// which means: players with ID 4, 7, 12; in lobby ID 4
//
std::string Game::MsgGetLobbyStatus() {
	std::string r = "lobbystatus|";
	for (size_t i = 0; i < vPlayers.size(); i++) {
		r.append(std::to_string(vPlayers[i]->GetId()));
		if (vPlayers[i]->GetId() == iGameHostId) r.append("H");
		r.append("|");
	}

	r.append("*");
	r.append(std::to_string(GetId()));
	r.append("-");

	return r;
}

Game::~Game() {

}