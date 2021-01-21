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
	bInGameOver = false;
	bInCardPlanning = false;
	bMyTurn = false;
	iId = -1;
	iCachedNumber = -1;
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

bool Game::IsInGameOver() {
	return bInGameOver;
}

bool Game::IsInCardPlanning() {
	return bInCardPlanning;
}

bool Game::IsMyTurn() {
	return bMyTurn;
}

int Game::GetCachedNumber() {
	return iCachedNumber;
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

void Game::SetInGameOver(bool state) {
	bInGameOver = state;
}

void Game::SetInCardPlanning(bool state) {
	bInCardPlanning = state;
}

void Game::SetMyTurn(bool state) {
	bMyTurn = state;
}

// Once the ID is set, it shouldn't be changed.
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

void Game::ClearCards() {
	vCards.clear();
}

void Game::AddCard(Card* card) {
	vCards.push_back(card);
}

Card* Game::GetCard(int id) {
	return vCards[id];
}

void Game::SetPlannedCard(Card* card, int num) {
	if(card) pCardPlanned = card;
	iCachedNumber = num;
}

Card* Game::GetPlannedCard() {
	if (pCardPlanned) return pCardPlanned;
	return nullptr;
}

void Game::UnsetPlannedCard() {
	pCardPlanned = nullptr;
}