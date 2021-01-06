#include <iostream>
#include "CScreen.h"
#include "CGame.h"
#include <string>
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

#include <stdio.h>
#include <io.h> //for _setmode
#include <fcntl.h> //for _O_U16TEXT

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
	MoveWindow(console, r.left, r.top, 800, 500, TRUE);
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
	PRINT("Makao");
	PRINT("Kliknij cyfre odpowiadajaca Twojemu wyborowi:");
	PRINT("1. Utworz pokoj:");
	PRINT("2. Dolacz do pokoju");
	PRINT("3. Wyjdz");
	FINISHPRINT;
}

// 5|8|6|*2-
void Screen::DisplayLobby(std::string datain) {
	int iEndOfPlayers = datain.find("*");
	int iEndOfMessage = datain.find("-");
	std::string roomnumber = datain.substr(iEndOfPlayers + 1, 1);
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
	if (isHost) PRINT("1. Rozpocznij gre");
	PRINT("0. Wyjdz z pokoju");
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

		PRINT(num << " Pokoj #" << rid << " (" << rpn << " graczy)");
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
	PRINT("0. Wroc do ekranu glownego");
	FINISHPRINT;
	return l;
}

void Screen::DisplayWaitScreen() {
	PRINT("Laczenie z serwerem. Prosze czekac.");
	FINISHPRINT;
}

// gamestatus|	1|2|3|		*10|		32|		43|		CA|S0|DK|C4|	*HQ|	X|			0-
// gamestatus|	...|...|	*gameid|	iturn|	time|	...|...|		*card|	function|	end-﻿
void Screen::DisplayGameScreen(std::string datain) {
	std::string workingdata = datain;
	//first cut off the junk data after '-'
	workingdata = workingdata.substr(0, workingdata.find("-") + 1);

	//print player list
	{
		PRINT("Players:");
		int endOfPlayerList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfPlayerList);
		int playerindicator = 1;
		while (true) {
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);
			PRINT(playerindicator << ". " << tempstring);
			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
		}
		workingdata = workingdata.substr(endOfPlayerList + 1, workingdata.length());
	}

	//print gameid
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Game ID: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print turn count
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Turn number: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print remaining time
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Remaining time: " << tempstring << "s");
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print player cards
	{
		PRINT("Player cards:");
		int endOfCardList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfCardList);
		int playerindicator = 1;
		while (true) {
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);
			cout << playerindicator << ". ";

			if (tempstring.rfind("C", 0) == 0) {
				setcolor(112);
				PRINT_SUIT(CLUB);
			}
			else if (tempstring.rfind("H", 0) == 0) {
				setcolor(116);
				PRINT_SUIT(HEART);
			}
			else if (tempstring.rfind("S", 0) == 0) {
				setcolor(112);
				PRINT_SUIT(SPADE);
			}
			else if (tempstring.rfind("D", 0) == 0) {
				setcolor(116);
				PRINT_SUIT(DIAMOND);
			}

			cout << tempstring.substr(1, tempstring.length());
			setcolor(7);
			PRINT(""); //fill
			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
		}
		workingdata = workingdata.substr(endOfCardList + 1, workingdata.length());
	}

	//print current card
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Current card: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print card function
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Card function: " << tempstring);
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print: has the game ended?
	{
		int sap = workingdata.find("-");
		std::string tempstring = workingdata.substr(0, sap);
		PRINT("Has the game ended?: " << (bool)stoi(tempstring) ? "Yes" : "No");
		//if ((bool)stoi(tempstring)) cout << "Yes"; else cout << "No";
		//cout << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	FINISHPRINT;
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
		for (size_t i = 0; i < linesToDo; i++) {
			for (size_t i = 0; i < spacesToDo; i++) {
				r.append(" ");
			}
		}
		r.append("\n");
		cout << r;

		setCursorPosition(0, 0);
	}
}