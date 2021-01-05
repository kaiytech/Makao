#include "Player.h"
#include <algorithm>

Player::Player(int id) {
	iId = id;
}

int Player::GetId() {
	return iId;
}

void Player::AddCard(Card* card) {
	vCards.push_back(card);
}

// returns true on success, false on failure
bool Player::RemoveCard(Card* card) {
	Msg("[P#" << GetId() << "] Removing card " << card->GetString());
	if (HasCard(card)) {
		std::vector<Card*>::iterator pos = std::find(vCards.begin(), vCards.end(), card);
		vCards.erase(pos);
		Success("[P#" << GetId() << "] Card removed.");
		return true;
	}
	else {
		Warn("[P#" << GetId() << "] Attempted to remove a card player doesn't have!");
		return false;
	}
}

bool Player::HasCard(Card* card) {
	std::vector<Card*>::iterator pos = std::find(vCards.begin(), vCards.end(), card);
	if (pos != vCards.end()) {
		return true;
	}
	else return false;
}

// returns card if player has it
// returns null if not
Card* Player::HasCard(std::string datain) {
	for (size_t i = 0; i < vCards.size(); i++) {
		if (datain.compare(vCards[i]->GetString()) == 0) {
			return vCards[i];
		}
	}
	return NULL;
}

Card* Player::GetCard(int index) {
	Card* c;
	if (vCards.empty()) return NULL;
	if (index < 0 || index > vCards.size() - 1) return NULL;
	c = vCards.at(index);
	if (!c) return NULL;
	return c;
}

int Player::GetCardAmount() {
	int c = 0;
	for (size_t i = 0; i < vCards.size(); i++) {
		c++;
	}
	return c;
}

std::string Player::GetCards() {
	std::string out = "";
	for (size_t i = 0; i < vCards.size(); i++) {
		out.append(vCards[i]->GetString());
		out.append("|");
	}
	out.append("*");
	return out;
}