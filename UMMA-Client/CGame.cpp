#include "CGame.h"

static Game* g = 0;
Game* GetGame() {
	if (!g) g = new Game();
	return g;
}

Game::Game() {
	bInLobby = false;
	bInGame = false;
	bInLobbyList = false;
	iId = -1;
}

bool Game::IsInLobby() {
	return bInLobby;
}

bool Game::IsInGame() {
	return bInGame;
}

bool Game::IsInLobbyList() {
	return bInLobbyList;
}

void Game::SetInLobby(bool state) {
	bInLobby = state;
}

void Game::SetInGame(bool state) {
	bInGame = state;
}

void Game::SetInLobbyList(bool state) {
	bInLobbyList = state;
}

// Once the ID is set, it can't be changed.
void Game::AssignId(int id) {
	if (iId == -1) iId = id;
}

bool Game::HasId() {
	if (iId == -1) return false;
	else return true;
}

int Game::GetId() {
	return iId;
}