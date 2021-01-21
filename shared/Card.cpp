#include "Card.h"

#ifdef CLIENT
#include "CGame.h"
#endif

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
	else if (eType == TYPE_K) { cout << "   WW"; }
	else if (eType == TYPE_J || eType == TYPE_Q) { cout << "   ww"; }
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
	else if (eType == TYPE_J || eType == TYPE_K) { cout << "   ^ {)"; }
	else if (eType == TYPE_Q) { cout << "   ^ {("; }
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
	else if (eType == TYPE_J) { cout << "  ("; PrintSuit();  cout << ")% "; }
	else if (eType == TYPE_Q || eType == TYPE_K) { cout << "  ("; PrintSuit();  cout << ")%%"; }
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
	else if (eType == TYPE_J) { cout << "   | % "; }
	else if (eType == TYPE_Q || eType == TYPE_K) { cout << "   |%%%"; }
	else { cout << "       "; }

	// drop shadow
	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);

	// fifth line of card
	screen->setCursorPosition(c.X, c.Y + 4);
	if (eType == TYPE_J) { cout << "    %%["; }
	else if (eType == TYPE_Q) { cout << "   %%%O"; }
	else if (eType == TYPE_K) { cout << "   %%%>"; }
	else { cout << "       "; }

	screen->setcolor(8);
	cout << char(219);
	screen->setcolor(color);

	screen->setCursorPosition(c.X, c.Y + 5);
	screen->setcolor(8);
	cout << " ";
	cout << char(223) << char(223) << char(223) << char(223) << char(223) << char(223) << char(223);
	screen->setcolor(7);

	screen->setCursorPosition(0, c.Y + 6);
	
	//screen->setCursorPosition(c.X + 8, c.Y);

}

// prints the small card right where the cursor is
// moves cursor to the right, where the next small card should be
void Card::PrintSmall(int i) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = screen->GetConsoleCursorPosition(hOut);

	int startx, starty;
	startx = c.X;
	starty = c.Y;

	int color = 0;
	if (eSuit == SUIT_CLUB || eSuit == SUIT_SPADE) color = 112;
	else color = 116;

	if (!game->IsInCardPlanning()) {
		game->IsMyTurn() ? cout << i << "." : cout << " ";
	}
	screen->setCursorPosition(c.X, c.Y + 1);
	screen->setcolor(color);
	if (!game->IsInCardPlanning()) {
		PrintSuit();
		cout << GetTypeText();
		if (eType == TYPE_10) cout << " ";
		else cout << "  ";

		screen->setcolor(8);
		cout << char(220);
	}
	screen->setcolor(7);

	screen->setCursorPosition(startx + 5, starty);
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

Card* Card::GetCardFromString(std::string datain) {
	char csuit = datain[0];
	char ctype = datain[1];

	CARD_SUIT s;
	CARD_TYPE t;
	bool fail = false;

	switch (csuit) {
		case 67: s = SUIT_CLUB; break;
		case 68: s = SUIT_DIAMOND; break;
		case 72: s = SUIT_HEART; break;
		case 83: s = SUIT_SPADE; break;
		default: fail = true; break;
	}

	switch (ctype) {
		case 65: t = TYPE_A; break;
		case 50: t = TYPE_2; break;
		case 51: t = TYPE_3; break;
		case 52: t = TYPE_4; break;
		case 53: t = TYPE_5; break;
		case 54: t = TYPE_6; break;
		case 55: t = TYPE_7; break;
		case 56: t = TYPE_8; break;
		case 57: t = TYPE_9; break;
		case 48: t = TYPE_10; break;
		case 74: t = TYPE_J; break;
		case 81: t = TYPE_Q; break;
		case 75: t = TYPE_K; break;
		default: fail = true; break;
	}
	if (fail) return NULL;

	Card* c = new Card(t, s);
	if (c) return c;
	else return NULL;
}

void Card::PrintMinimal() {
	int color;
	if (eSuit == SUIT_CLUB || eSuit == SUIT_SPADE) color = 112;
	else color = 116;
	screen->setcolor(color);
	cout << GetTypeText();
	PrintSuit();
	screen->setcolor(7);
}

#endif // CLIENT
