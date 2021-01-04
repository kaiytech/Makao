#include "Game.h"
#include <stdlib.h>
#include "SessionHandler.h"
#include <time.h>

Game::Game(int id) {
	iId = id;
	iType = GAME_NONE;
	iGameHostId = -1;
	iTurn = 1;
	iPlayerTurnId = -1;
	iTurnEndTime = -1;
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

bool Game::MakeGame() {
	iType = GAME_GAME;
	PrepareCards();
	return true;
}

void Game::PrepareCards() {
	//todo: count how many decks do we need.
	AddDeck();
	Shuffle();
	Deal();
	PutOnTop(FindCardToPutOnTop());
	ExecuteFirstMove();
}

void Game::Shuffle() {
	std::random_shuffle(vDeck.begin(), vDeck.end());
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

Card* Game::GetCardOnTop() {
	return pCardOnTop;
}



// executes the move (or doesn't, if not needed)
std::string Game::ExecuteMove(std::string datain) {
	if (datain.rfind("first", 0) == 0) { //first round
		iPlayerTurnId = vPlayers[0]->GetId(); // the first player begins
		time_t t = time(NULL);
		iTurnEndTime = t + TURN_TIME;
	}
	return "";
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

std::string Game::MsgGetGameStatus(int playerid) {
	std::string out = "gamestatus|";
	Player* p = GetSessionHandler()->GetPlayer(playerid);
	Card* cot = GetCardOnTop();

	// playerlist
	for (size_t i = 0; i < vPlayers.size(); i++) {
		out.append(std::to_string(vPlayers[i]->GetId()));
		if (vPlayers[i]->GetId() == iGameHostId) out.append("H");
		if (vPlayers[i]->GetId() == iPlayerTurnId) out.append("T");
		out.append("|");
	}
	out.append("*");

	//gameid
	out.append(std::to_string(GetId()));
	out.append("|");

	//iturn
	out.append(std::to_string(iTurn));
	out.append("|");

	//time
	time_t t = time(NULL);
	long long int currentTime = t;
	long long int remainingTime = iTurnEndTime - currentTime;
	out.append(std::to_string(remainingTime));
	out.append("|"); //wip

	//cards
	//todo: if(p)
	out.append(p->GetCards());

	//current card
	//todo if(cot)
	out.append(cot->GetString());
	out.append("|");

	//card function
	out.append("X|"); //wip

	//end?
	out.append("0-"); //wip

	return out;
	//return "gamestatus|1|2|3|*10|32|43|CA|S0|DK|C4|*HQ|X|0-";
}

void Game::AddDeck() {
	int c = 0;
	for (size_t type = 0; type < TYPE_LENGTH; type++) {
		for (size_t suit = 0; suit < SUIT_LENGTH; suit++) {
			// make a new card
			vCards.push_back(new Card(static_cast<CARD_TYPE>(type), static_cast<CARD_SUIT>(suit)));
			
			// and push it to the deck
			vDeck.push_back(vCards[c]);
			c++;
		}
	}
}

void Game::Deal() {
	int c = 0;
	for (size_t i = 0; i < vPlayers.size(); i++) {
		for (size_t s = 0; s < 5; s++){
			TransferCardToPlayer(vDeck[0], vPlayers[i]);
		}
	}
}

void Game::TransferCardToPlayer(Card* card, Player* player) {
	if (!player) return;
	if (!card) return;

	player->AddCard(card);

	std::vector<Card*>::iterator pos = std::find(vDeck.begin(), vDeck.end(), card);
	if (pos != vDeck.end()) {
		vDeck.erase(pos);
	}
	else {
		// if this happens, something's horribly wrong!!
		// abort();
		return;
	}
}

Card* Game::FindCardToPutOnTop() {
	for (size_t i = 0; i < vDeck.size(); i++) {
		if (vDeck[i]->CanBePutOnTop()) return vDeck[i];
	}

	// if we didn't find the card to put on top,
	// then we add one deck and try once again
	AddDeck();
	return FindCardToPutOnTop();
}

void Game::PutOnTop(Card* card) {
	//pull the current card from the top
	if (pCardOnTop) {
		vDeck.push_back(pCardOnTop);
	}
	pCardOnTop = card;
}

void Game::ExecuteFirstMove() {
	ExecuteMove("first");
}

Game::~Game() {

}