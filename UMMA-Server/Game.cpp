#include "Game.h"
#include <stdlib.h>
#include "SessionHandler.h"
#include <chrono>

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
	Msg("[G#" << GetId() << "] Demoting game to lobby");
	iType = GAME_LOBBY;
	Success("[G#" << GetId() << "] Game demoted to lobby!");
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
	Msg("[G#" << GetId() << "] Setting host to player #" << iGameHostId);
	Success("[G#" << GetId() << "] Host set!");
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
	Msg("[G#" << GetId() << "] Promoting lobby to game");
	iType = GAME_GAME;
	PrepareCards();
	Success("[G#" << GetId() << "] Lobby promoted to game!");
	return true;
}

void Game::PrepareCards() {
	//todo: count how many decks do we need.
	Msg("[G#" << GetId() << "] Preparing cards...");
	AddDeck();
	Shuffle();
	Deal();
	PutOnTop(FindCardToPutOnTop());
	ExecuteFirstMove();
}

void Game::Shuffle() {
	Msg("[G#" << GetId() << "] Shuffling cards...");
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
	Msg("[G#" << GetId() << "] Adding player to the game...");
	if (!player) {
		Error("[G#" << GetId() << "] Invalid player!");
		return false;
	}

	int pid = player->GetId();

	// check if the player isn't already in the game
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == pid) {
			Msg("[G#" << GetId() << "] Player already in game.");
			return false; // and return false if is
		}
	}

	vPlayers.push_back(player);
	Success("[G#" << GetId() << "] Player #" << player->GetId() << " added to the game");
	return true; // success. ...presumably...
}

//todo
bool Game::RemovePlayer(Player *player) {
	return true;
}

bool Game::RemovePlayer(int playerid) {
	Msg("[G#" << GetId() << "] Removing player from the game...");
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == playerid) vPlayers.erase(vPlayers.begin() + i);
	}
	Success("[G#" << GetId() << "] Player #" << playerid << " removed from the game");
	return true; //hm.
}

Card* Game::GetCardOnTop() {
	return pCardOnTop;
}



// executes the move (or doesn't, if not needed)
std::string Game::ExecuteMove(std::string datain) {
	Msg("[G#" << GetId() << "] Executing move...");
	if (datain.rfind("first", 0) == 0) { //first round
		Msg("[G#" << GetId() << "] This is a first move!");
		iPlayerTurnId = vPlayers[0]->GetId(); // the first player begins
		std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds> (
			std::chrono::system_clock::now().time_since_epoch()
			);
		iTurnEndTime = s.count() + TURN_TIME;
		Success("[G#" << GetId() << "] Move execution finished!");
		return "AFS";
	}

	// playing the card
	if (datain.rfind("playcard|", 0) == 0) {
		Msg("[G#" << GetId() << "] Playing card...");
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

		Player* pPlayer = GetSessionHandler()->GetPlayer(playerid);
		if (!pPlayer) {
			Warn("[G#" << GetId() << "] Can't finish move: unknown player");
			return "AFS";
		}

		if (iPlayerTurnId != pPlayer->GetId()) {
			Warn("[G#" << GetId() << "] Can't finish move: it's not player #" << pPlayer->GetId() << "s move!");
			return "AFS";
		}
		
		if (!IsPlayerInGame(pPlayer)) {
			Warn("[G#" << GetId() << "] Can't finish move: player not in game");
			return "AFS";
		}
		
		s = s.substr(sap + 1, s.length());
		sap = s.find("|");
		std::string card = s.substr(0, sap);
		
		Card* c = pPlayer->GetCard(stoi(s)-1);
		if (!c) {
			Warn("[G#" << GetId() << "] Can't finish move: player tried to play unknown card");
			return "AFS";
		}

		if (!Validate(c)) {
			Warn("[G#" << GetId() << "] Can't finish move: player tried to play mismatched card");
			return "AFS";
		}

		PutOnTop(c);
		if (!pPlayer->RemoveCard(c)) {
			Warn("[G#" << GetId() << "] Can't finish move: Player doesn't have the card");
			return "AFS";
		}

		std::chrono::seconds s2 = std::chrono::duration_cast<std::chrono::seconds> (
			std::chrono::system_clock::now().time_since_epoch()
			);
		iTurnEndTime = s2.count() + TURN_TIME;

		PassTurn();
		Success("[G#" << GetId() << "] Move execution finished!");
		return MsgGetGameStatus(pPlayer->GetId());
	}

	if (datain.rfind("drawcard|", 0) == 0) {
		Msg("[G#" << GetId() << "] Drawing a card...");
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

		Player* pPlayer = GetSessionHandler()->GetPlayer(playerid);
		if (!pPlayer) {
			Warn("[G#" << GetId() << "] Can't finish move: unknown player");
			return "AFS";
		}

		if (pPlayer->GetId() != iPlayerTurnId) {
			Warn("[G#" << GetId() << "] Can't finish move: it's not player #" << pPlayer->GetId() << "s move!");
			return "AFS";
		}

		Card* card = vDeck[0];
		if (!card) {
			Error("[G#" << GetId() << "] Can't finish move: can't pull a new card!");
			return "AFS";
		}

		TransferCardToPlayer(card, pPlayer);

		std::chrono::seconds s3 = std::chrono::duration_cast<std::chrono::seconds> (
			std::chrono::system_clock::now().time_since_epoch()
			);
		iTurnEndTime = s3.count() + TURN_TIME;

		PassTurn();
		Success("[G#" << GetId() << "] Move execution finished!");
		return MsgGetGameStatus(pPlayer->GetId());
	}



	Success("[G#" << GetId() << "] Move execution finished!");
	return "AFS";
}

bool Game::Validate(Card* card) {
	if (
		card->GetSuit() == pCardOnTop->GetSuit()
		||
		card->GetType() == pCardOnTop->GetType()
		) return true;

	else return false;
}


void Game::PassTurn() {
	int iTurn = -1;
	for (int i = 0; i < vPlayers.size(); i++) {
		if (vPlayers[i]->GetId() == iPlayerTurnId) iTurn = i;
	}
	if (iTurn == -1) return;

	if (iTurn == vPlayers.size()-1) iPlayerTurnId = vPlayers[0]->GetId();
	else iPlayerTurnId = vPlayers[iTurn + 1]->GetId();

	return;
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

	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds> (
		std::chrono::system_clock::now().time_since_epoch()
		);
	long long int currentTime = s.count();
	long long int remainingTime = iTurnEndTime - currentTime;
	if (remainingTime < 1) {
		iTurnEndTime = s.count() + TURN_TIME;
		PassTurn();
		return MsgGetGameStatus(playerid);
	}

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
	out.append(std::to_string(remainingTime));
	out.append("|");

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
	Msg("[G#" << GetId() << "] Generating a new card deck...");
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
		Msg("[G#" << GetId() << "] Dealing player #" << vPlayers[i] << " with 5 cards");
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
	Msg("[G#" << GetId() << "] Putting a card on top");
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