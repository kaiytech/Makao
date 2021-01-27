#include <iostream>
#include "CScreen.h"
#include "CGame.h"
#include <string>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

#include <stdio.h>

using namespace std;

// hacky
// #define list(x,y) l.l#x = y

#define PRINT(x) cout << x; Fill(true)
#define FINISHPRINT Fill(false)

static Screen* sc = 0;
Screen* GetScreen() {
	if (!sc) sc = new Screen();
	return sc;
}

Screen::Screen() {
	// remove the blinking cursor
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
	// </>

	// font size
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 24;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	// console size
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 720, 500, TRUE);
}


void Screen::DisplaySplashScreen() {
	windowsize w = GetWindowSize();
	int x = w.x; int y = w.y;
	std::vector<std::string> s;
	setcolor(40);
	for (size_t i = 1; i < (y - 8) / 2; i++) {
		for (size_t i = 0; i <= x; i++)
		{
			cout << (char)176;
		}
		cout << "\n";
	}
	setcolor(7);
	s.push_back("dummy");
	s.push_back("  ___  ___      _                ");
	s.push_back("  |  \\/  |     | |               ");
	s.push_back("  | .  . | __ _| | ____ _  ___   ");
	s.push_back("  | |\\/| |/ _` | |/ / _` |/ _ \\  ");
	s.push_back("  | |  | | (_| |   < (_| | (_) | ");
	s.push_back("  \\_|  |_/\\__,_|_|\\_\\__,_|\\___/  ");
	s.push_back("");
	s.push_back("  Coding: Kajetan Krawczyk");
	s.push_back("  Testing: Michal Radtke");
	s.push_back("");
	s.push_back("  \x1B[5m> Press any key to start <");
	s.push_back("                                ");
	for (size_t fi = 1; fi < s.size(); fi++) {
		setcolor(40);
		for (size_t i = 0; i < (x  - 36)/2; i++) cout << (char)176;
		setcolor(112);
		cout << s[fi] << "\n";
		setcolor(7);
	}
	FINISHPRINT;
}

void Screen::DisplayMainMenu() {
	if (game->IsInGame()) return;
	PRINT("Makao");
	PRINT("Press 1-4 to interact with menu:");
	PRINT("1. New lobby");
	PRINT("2. Lobby list");
	PRINT("3. How to play?");
	PRINT("4. Exit");
	FINISHPRINT;
}

// 5|8|6|*2-
void Screen::DisplayLobby(std::string datain) {
	int iEndOfPlayers = datain.find("*");
	int iEndOfMessage = datain.find("-");
	std::string roomnumber = datain.substr(iEndOfPlayers + 1, datain.length());
	roomnumber = roomnumber.substr(0, roomnumber.find("-"));
	PRINT("Pokoj ID: " << roomnumber);
	bool isHost = false;

	std::string players = datain.substr(0, iEndOfPlayers);
	while (true) {
		if (players.find("|") == string::npos) break;
		int sap = players.find("|");
		std::string pl = players.substr(0, sap);
		if (pl.find("H") != std::string::npos) {
			//cout << ">> Player #" << pl.substr(0, pl.length() - 1) << " (Host)" << "\n";
			PRINT(">> Player #" << pl.substr(0, pl.length() - 1) << " (Host)");
			if (stoi(pl.substr(0, pl.length() - 1)) == game->GetId()) isHost = true;
		}
		else {
			//cout << ">> Player #" << pl << "\n";
			PRINT(">> Player #" << pl);
		}
		players = players.substr(sap + 1, players.length());
	}
	PRINT("");
	if (isHost) PRINT("1. Start game");
	PRINT("0. Leave lobby");
	FINISHPRINT;
}

// lobbylist|id+players|id+players|-
// returns lobbylist
lobbylist Screen::DisplayLobbyList(std::string datain) {
	lobbylist l;
	l.l1 = -1; l.l2 = -1; l.l3 = -3; l.l4 = -1; l.l5 = -1; l.l6 = -1; l.l7 = -1; l.l8 = -1; l.l9 = -1;
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

		PRINT(num << " Lobby #" << rid << " (" << rpn << "/6 players)");
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
	PRINT("0. Back to main menu");
	FINISHPRINT;
	return l;
}

void Screen::DisplayWaitScreen() {
	//PRINT("Laczenie z serwerem. Prosze czekac.");
	//FINISHPRINT;
}

//endgame|winid-
void Screen::DisplayGameOverScreen(std::string datain) {
	std::string out = datain;
	int sap = out.rfind("|");
	int end = out.rfind("-");
	out = out.substr(sap + 1, end);

	int playerid = stoi(out);
	if (playerid == game->GetId()) {
		PRINT("You won!");
	} else {
		PRINT("You lost!");
	}
	PRINT("Player #" << playerid << " wins the game!");
	FINISHPRINT;
}

// don't use frequently as it causes blinking!!!
void Screen::ClearScreen() {
	for (size_t y = 0; y < GetWindowSize().y; y++){
		for (size_t x = 0; x < GetWindowSize().x; x++){
			cout << " ";
		}
		cout << "\n";
	}
}

// gamestatus|	1|2|3|		*10|		32|		43|		CA|S0|DK|C4|	*HQ|	X|			0-
// gamestatus|	...|...|	*gameid|	iturn|	time|	...|...|		*card|	function|	end-﻿
void Screen::DisplayGameScreen(std::string datain) {
	std::string workingdata = datain;
	//first cut off the junk data after '-'
	workingdata = workingdata.substr(0, workingdata.find("-") + 1);
	cout << "\n\n\n\n\n\n\n\n";
	//print player list
	{
		//PRINT("Players:");
		setCursorPosition(GetWindowSize().x - 14, 2);
		cout << "Players:";
		int endOfPlayerList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfPlayerList);
		int playerindicator = 1;
		int ypos = 3;
		while (true) {
			setCursorPosition(GetWindowSize().x - 14, ypos);
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);

			std::string host = "H"; bool bhost = false;
			std::string turn = "T"; bool bturn = false;
			if (tempstring.find(turn) != std::string::npos) {
				bturn = true;
				std::string totest = tempstring.substr(0, tempstring.find(turn));
				int id = game->GetId();
				if (stoi(totest) == game->GetId()) {
					game->SetMyTurn(true);
				}
				else {
					game->SetMyTurn(false);
				}
			}
			if (tempstring.find(host) != std::string::npos) { 
				bhost = true; 
			}
			if(bturn) tempstring.erase(std::remove(tempstring.begin(), tempstring.end(), 'T'), tempstring.end());
			if(bhost) tempstring.erase(std::remove(tempstring.begin(), tempstring.end(), 'H'), tempstring.end());

			tempstring = std::string("#").append(tempstring);

			if (bturn) tempstring = std::string(">> ").append(tempstring);
			if (bhost) tempstring.append(" (Host)");
			//cout << tempstring;
			PRINT(tempstring);

			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
			ypos++;
		}
		workingdata = workingdata.substr(endOfPlayerList + 1, workingdata.length());
	}

	//print gameid
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		setCursorPosition(0, 0);
		game->IsMyTurn() ? setcolor(32) : setcolor(128);
		cout << char(254);
		PRINT(" |MAKAO|    Game #" << tempstring);
		setcolor(8);

		for (size_t i = 0; i < GetWindowSize().x; i++) {
			cout << char(223);
		}

		setcolor(7);
		//PRINT("Game ID: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print turn count
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		//PRINT("Turn number: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print remaining time
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		setCursorPosition(GetWindowSize().x-14, 10);
		PRINT("Time left: " << tempstring << "s");
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print player cards
	{
		setCursorPosition(0, 14);
		PRINT(""); PRINT("");
		setCursorPosition(0, 14);

		//PRINT("Player cards:");
		int endOfCardList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfCardList);
		int playerindicator = 1;
		cout << " ";

		game->ClearCards(); // remember to clear the cached card list first.

		int printedcards = 0;
		while (true) {
			if (printedcards >= 9) break;
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);
			Card* c = Card::GetCardFromString(tempstring);
			game->AddCard(c); // add to card list
			if (c && !game->IsInCardPlanning()) c->PrintSmall(playerindicator);
			printedcards++;
			//delete c;
			// ^THAT LINE UNCOMMENTED WASTED ME AN HOUR OF MY PRECIOUS LIFE
			// IM SERIOUS
			// KAWEFJIWEJFGJ2q34e23r34rEWsaJdOGVJHREJsdsaGIOPE

			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
		}

		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		setCursorPosition(0, GetConsoleCursorPosition(hOut).Y);
		if (game->IsInCardPlanning()) {
			Card* c = game->GetPlannedCard();
			if (c) {
				cout << "Attempting to play ";
				c->PrintMinimal();
				switch (game->GetPlannedCard()->GetType())
				{
				case TYPE_J:
					PRINT(". What card do you demand?");
					PRINT("1:10  2:2  3:3  4:4  5:5  6:6  7:7  8:8  9:9");
					break;
				case TYPE_A:
				{
					PRINT(". What suit do you demand?");
					Card* c1 = new Card(TYPE_A, SUIT_CLUB);
					Card* c2 = new Card(TYPE_A, SUIT_SPADE);
					Card* c3 = new Card(TYPE_A, SUIT_DIAMOND);
					Card* c4 = new Card(TYPE_A, SUIT_HEART);
					cout << "1:"; c1->PrintSuit();
					cout << " 2:"; c2->PrintSuit();
					cout << " 3:"; c3->PrintSuit();
					cout << " 4:"; c4->PrintSuit();
					PRINT(" ");
					//setCursorPosition(0, GetConsoleCursorPosition(hOut).Y + 1);
				}
				default:
					setCursorPosition(0, GetConsoleCursorPosition(hOut).Y + 2);
					break;
				}

			}
		}
		else {
			setCursorPosition(0, GetConsoleCursorPosition(hOut).Y + 2);
		}
		setcolor(6);
		windowsize w = GetWindowSize();
		for (size_t i = 0; i < w.x; i++){
			cout << char(223);
		}
		cout << "\n";

		if (game->IsInCardPlanning()) {
			//setCursorPosition(0, GetConsoleCursorPosition(hOut).Y + -1);
			PRINT(" (1-9) - demand       (0) - cancel              ");
			FINISHPRINT;
		}
		else {
			PRINT(" (1-9) - play card                              ");
			cout << " (z)   - draw card    (x) - forfeit             ";
		}
		setcolor(7);

		workingdata = workingdata.substr(endOfCardList + 1, workingdata.length());
	}

	Card* currentCard;
	//print current card
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		//PRINT("Current card: " << tempstring);

		setcolor(7);
		setCursorPosition(4, 4);
		Card* c = Card::GetCardFromString(tempstring);
		currentCard = c;
		if(c) c->PrintBig();

		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print card function
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		if (tempstring.find("X") != std::string::npos) {
			cout << "               ";
		}
		else {
			cout << "  Demand: ";
			int si = stoi(tempstring);
			if (currentCard->GetType() == TYPE_A) {
				if (si == 1) { Card* c = new Card(TYPE_J, SUIT_CLUB); c->PrintSuit(); }
				else if (si == 2) { Card* c = new Card(TYPE_J, SUIT_SPADE); c->PrintSuit(); }
				else if (si == 3) { Card* c = new Card(TYPE_J, SUIT_DIAMOND); c->PrintSuit(); }
				else if (si == 4) { Card* c = new Card(TYPE_J, SUIT_HEART); c->PrintSuit(); }
			}
			else if (currentCard->GetType() == TYPE_J) {
				cout << si;
			}
		}
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print: war?
	{
		int sap = workingdata.find("-");
		std::string tempstring = workingdata.substr(0, sap);
		if (stoi(tempstring) > 1) {
			static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD crd = GetConsoleCursorPosition(hOut);
			setCursorPosition(0, crd.Y);
			cout << "  War: " << stoi(tempstring) << " cards.";
		}
		else {
			cout << "            ";
		}
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	screen->setCursorPosition(0, 0);

	//FINISHPRINT;
}


COORD Screen::GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void Screen::setCursorPosition(int x, int y) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

windowsize Screen::GetWindowSize() {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	GetConsoleScreenBufferInfo(hOut, &cbsi);
	windowsize w;
	w.x = cbsi.srWindow.Right - cbsi.srWindow.Left;
	w.y = cbsi.srWindow.Bottom - cbsi.srWindow.Top;
	return w;
}

void Screen::Fill(bool line = true) {
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = GetConsoleCursorPosition(hOut);

	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	GetConsoleScreenBufferInfo(hOut, &cbsi);

	if (line) {
		int spacesToDo = cbsi.srWindow.Right - cbsi.srWindow.Left - c.X;
		std::string r = "";
		for (size_t i = 0; i < spacesToDo; i++) {
			r.append(" ");
		}
		r.append("\n");
		cout << r;
	}
	else {
		int linesToDo = cbsi.srWindow.Bottom - cbsi.srWindow.Top - c.Y;
		int spacesToDo = cbsi.srWindow.Right - cbsi.srWindow.Left;
		std::string r = "";
		if (linesToDo > 0) {
			for (size_t i = 0; i < linesToDo; i++) {
				for (size_t i = 0; i < spacesToDo; i++) {
					r.append(" ");
				}
			}
		}
		r.append("\n");
		cout << r;

		setCursorPosition(0, 0);
	}
}