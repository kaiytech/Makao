#include "Card.h"

Card::Card(CARD_TYPE type, CARD_SUIT suit) {
	eType = type;
	eSuit = suit;
}

CARD_TYPE Card::GetType() {
	return eType;
}

CARD_SUIT Card::GetSuit() {
	return eSuit;
}

std::string Card::GetString() {
	std::string out = "";

	switch (eSuit) {
		case SUIT_CLUB:		out.append("C"); break;
		case SUIT_DIAMOND:	out.append("D"); break;
		case SUIT_HEART:	out.append("H"); break;
		case SUIT_SPADE:	out.append("S"); break;
		default:			break;
	}

	switch (eType) {
		case TYPE_A:	out.append("A"); break;
		case TYPE_J:	out.append("J"); break;
		case TYPE_Q:	out.append("Q"); break;
		case TYPE_K:	out.append("K"); break;
		case TYPE_2:	out.append("2"); break;
		case TYPE_3:	out.append("3"); break;
		case TYPE_4:	out.append("4"); break;
		case TYPE_5:	out.append("5"); break;
		case TYPE_6:	out.append("6"); break;
		case TYPE_7:	out.append("7"); break;
		case TYPE_8:	out.append("8"); break;
		case TYPE_9:	out.append("9"); break;
		case TYPE_10:	out.append("0"); break;
		default:		break;
	}
	return out;
}

bool Card::CanBePutOnTop() {
	if (
		eType == TYPE_5 ||
		eType == TYPE_6 ||
		eType == TYPE_7 ||
		eType == TYPE_8 ||
		eType == TYPE_9 ||
		eType == TYPE_10 ||
		eType == TYPE_Q
		) return true;
	return false;
}