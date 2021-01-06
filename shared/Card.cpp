#include "Card.h"

using namespace std;

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

#ifdef CLIENT

// prints the big card right where the cursor is
// moves cursor just below the card to position (0,0)
void Card::PrintBig() {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = screen->GetConsoleCursorPosition(hOut);

	int startx, starty;
	startx = c.X;
	starty = c.Y;

	int color = 0;
	if (eSuit == SUIT_CLUB || eSuit == SUIT_SPADE) color = 112;
	else color = 116;
	
	// first line of card
	screen->setcolor(color);
	PrintSuit();
	cout << GetTypeText();
	if (eType == TYPE_10) { cout << "    "; }
	else {					cout << "     "; }

	// drop shadow
	screen->setcolor(8);
	cout << char(220);
	screen->setcolor(color);


	// second line of card
	screen->setCursorPosition(c.X, c.Y + 1);
	if (eType == TYPE_2 || eType == TYPE_3) { cout << "   "; PrintSuit(); cout << "   "; }
	else if (eType == TYPE_4 || eType == TYPE_5 || eType == TYPE_6) { cout << "  "; PrintSuit(); cout << " "; PrintSuit(); cout << "  "; }
	else if (eType == TYPE_7 || eType == TYPE_8 || eType == TYPE_9 || eType == TYPE_10) { cout << "  "; PrintSuit(); PrintSuit(); PrintSuit(); cout << "  "; }
	else { cout << "       "; }

	// drop shadow
	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);
	

	// third line of card
	screen->setCursorPosition(c.X, c.Y + 2);
	if (eType == TYPE_A || eType == TYPE_3 || eType == TYPE_5 || eType == TYPE_7) { cout << "   "; PrintSuit(); cout << "   "; }
	else if (eType == TYPE_6 || eType == TYPE_8) { cout << "  "; PrintSuit(); cout << " "; PrintSuit(); cout << "  "; }
	else if (eType == TYPE_10) { cout << " "; PrintSuit(); PrintSuit(); cout << " "; PrintSuit(); PrintSuit(); cout << " "; }
	else if (eType == TYPE_9) { cout << "  "; PrintSuit(); PrintSuit(); PrintSuit(); cout << "  "; }
	else cout << "       ";

	// drop shadow
	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);

	// fourth line of card (same as 2nd)
	screen->setCursorPosition(c.X, c.Y + 3);
	if (eType == TYPE_2 || eType == TYPE_3) { cout << "   "; PrintSuit(); cout << "   "; }
	else if (eType == TYPE_4 || eType == TYPE_5 || eType == TYPE_6) { cout << "  "; PrintSuit(); cout << " "; PrintSuit(); cout << "  "; }
	else if (eType == TYPE_7 || eType == TYPE_8 || eType == TYPE_9 || eType == TYPE_10) { cout << "  "; PrintSuit(); PrintSuit(); PrintSuit(); cout << "  "; }
	else { cout << "       "; }

	// drop shadow
	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);

	// fifth line of card
	screen->setCursorPosition(c.X, c.Y + 4);
	if (eType == TYPE_10) { cout << "       "; }
	else { cout << "       "; }

	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);

	screen->setCursorPosition(c.X, c.Y + 5);
	screen->setcolor(8);
	cout << " ";
	cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
	screen->setcolor(7);

	screen->setCursorPosition(c.X, c.Y + 6);
	
	//screen->setCursorPosition(c.X + 8, c.Y);

}

// prints the small card right where the cursor is
// moves cursor to the right, where the next small card should be
void Card::PrintSmall() {

}


std::string Card::GetTypeText() {
	std::string out;
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
	case TYPE_10:	out.append("10"); break;
	default:		break;
	}
	return out;
}

void Card::PrintSuit() {
	std::string out;
	switch (eSuit) {
	case SUIT_CLUB:		PRINT_SUIT(CLUB); break;
	case SUIT_DIAMOND:	PRINT_SUIT(DIAMOND); break;
	case SUIT_HEART:	PRINT_SUIT(HEART); break;
	case SUIT_SPADE:	PRINT_SUIT(SPADE); break;
	default:			break;
	}
}

#endif // CLIENT
