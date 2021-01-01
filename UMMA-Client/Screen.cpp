#include <iostream>
#include "Screen.h"
#include "Game.h"
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
	bool isHost = false;

	std::string players = datain.substr(0, iEndOfPlayers);
	while (true) {
		if (players.find("|") == string::npos) break;
		int sap = players.find("|");
		std::string pl = players.substr(0, sap);
		if (pl.find("H") != std::string::npos) {
			cout << ">> Player #" << pl.substr(0, pl.length() - 1) << " (Host)" << "\n";
			if (stoi(pl.substr(0, pl.length() - 1)) == game->GetId()) isHost = true;
		}
		else {
			cout << ">> Player #" << pl << "\n";
		}
		players = players.substr(sap + 1, players.length());
	}
	cout << "\n";
	if(isHost) cout << "1. Rozpocznij gre\n";
	cout << "0. Wyjdz z pokoju\n";
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

// gamestatus|	1|2|3|		*10|		32|		1|		43|		CA|S0|DK|C4|	*HQ|	X|			0-
// gamestatus|	...|...|	*gameid|	iturn|	turn|	time|	...|...|		*card|	function|	end-﻿
void Screen::DisplayGameScreen(std::string datain) {
	ClearScreen();

	std::string workingdata = datain;
	//first cut off the junk data after '-'
	workingdata = workingdata.substr(0, workingdata.find("-") + 1);

	//print player list
	{
		cout << "Players: \n";
		int endOfPlayerList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfPlayerList);
		int playerindicator = 1;
		while (true) {
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);
			cout << playerindicator << ". " << tempstring << "\n";
			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
		}
		workingdata = workingdata.substr(endOfPlayerList + 1, workingdata.length());
	}

	//print gameid
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Game ID: " << tempstring << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print turn count
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Turn number: " << tempstring << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print: is it my turn?
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Is it my turn?: ";
		if ((bool)stoi(tempstring)) cout << "Yes"; else cout << "No"; 
		cout << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print remaining time
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Remaining time: " << tempstring << "s\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print player cards
	{
		cout << "Player cards: \n";
		int endOfCardList = workingdata.find("*");
		std::string playerlist = workingdata.substr(0, endOfCardList);
		int playerindicator = 1;
		while (true) {
			if (playerlist.find("|") == string::npos) break;
			int sap = playerlist.find("|");
			std::string tempstring = playerlist.substr(0, sap);
			cout << playerindicator << ". " << tempstring << "\n";
			playerlist = playerlist.substr(sap + 1, playerlist.length());
			playerindicator++;
		}
		workingdata = workingdata.substr(endOfCardList + 1, workingdata.length());
	}

	//print current card
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Current card: " << tempstring << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print card function
	{
		int sap = workingdata.find("|");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Card function: " << tempstring << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	//print: has the game ended?
	{
		int sap = workingdata.find("-");
		std::string tempstring = workingdata.substr(0, sap);
		cout << "Has the game ended?: ";
		if ((bool)stoi(tempstring)) cout << "Yes"; else cout << "No";
		cout << "\n";
		workingdata = workingdata.substr(sap + 1, workingdata.length());
	}

	cout << workingdata << "\n";

}