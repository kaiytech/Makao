#include <iostream>
#include "Screen.h"
#include <string>

using namespace std;

// hacky
// #define list(x,y) l.l#x = y

static Screen* sc = 0;
Screen* GetScreen() {
	if (!sc) sc = new Screen();
	return sc;
}

Screen::Screen() {

}

void Screen::ClearScreen() {
	system("cls"); //literally the only thing that makes it Linux incompatible :'))
}

void Screen::DisplayMainMenu() {
	ClearScreen();
	cout << "MAKAO\n";
	cout << "Kliknij cyfre odpowiadajaca Twojemu wyborowi:\n";
	cout << "1. Utworz pokoj\n";
	cout << "2. Dolacz do pokoju\n";
	cout << "3. Wyjdz\n";
}

// 5|8|6|*2-
void Screen::DisplayLobby(std::string datain) {
	ClearScreen();
	int iEndOfPlayers = datain.find("*");
	int iEndOfMessage = datain.find("-");
	std::string roomnumer = datain.substr(iEndOfPlayers + 1, 1);
	cout << "Pokoj ID: " << roomnumer << ".\n";

	std::string players = datain.substr(0, iEndOfPlayers);
	while (true) {
		if (players.find("|") == string::npos) break;
		int sap = players.find("|");
		std::string pl = players.substr(0, sap);
		cout << ">> Player #" << pl << "\n";
		players = players.substr(sap + 1, players.length());
	}
	cout << "1. Rozpocznij gre\n";
	cout << "2. Wyjdz z pokoju\n";
}

// lobbylist|id+players|id+players|-
// returns lobbylist
lobbylist Screen::DisplayLobbyList(std::string datain) {
	lobbylist l;
	l.l1 = -1; l.l2 = -1; l.l3 = -3; l.l4 = -1; l.l5 = -1; l.l6 = -1; l.l7 = -1; l.l8 = -1; l.l9 = -1;
	ClearScreen();
	int iEndOfMessage = datain.find("-");
	string rooms = datain.substr(0, iEndOfMessage);
	int num = 1;
	while (true) {
		if (num > 9) break;
		if (rooms.find("|") == string::npos) break;
		int sap = rooms.find("|");

		std::string r = rooms.substr(0, sap);
		std::string rid = r.substr(0, r.find("+"));
		std::string rpn = r.substr(r.find("+") + 1, r.length());

		cout << num << " Pokoj #" << rid << " (" << rpn << " graczy)" << "\n";
		rooms = rooms.substr(sap + 1, rooms.length());

		// this is embarassing
		switch (num) {
		case 1: l.l1 = stoi(rid); break;
		case 2: l.l2 = stoi(rid); break;
		case 3: l.l3 = stoi(rid); break;
		case 4: l.l4 = stoi(rid); break;
		case 5: l.l5 = stoi(rid); break;
		case 6: l.l6 = stoi(rid); break;
		case 7: l.l7 = stoi(rid); break;
		case 8: l.l8 = stoi(rid); break;
		case 9: l.l9 = stoi(rid); break;
		default:
			break;
		}
		num++;
	}
	cout << "0. Wroc do ekranu glownego";
	return l;
}

void Screen::DisplayWaitScreen() {
	ClearScreen();
	cout << "Laczenie z serwerem. Prosze czekac.\n";
}