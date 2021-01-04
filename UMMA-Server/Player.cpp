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
	if (HasCard(card)) {
		std::vector<Card*>::iterator pos = std::find(vCards.begin(), vCards.end(), card);
		vCards.erase(pos);
		return true;
	}
	else return false;
}

bool Player::HasCard(Card* card) {
	std::vector<Card*>::iterator pos = std::find(vCards.begin(), vCards.end(), card);
	if (pos != vCards.end()) {
		return true;
	}
	else return false;
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