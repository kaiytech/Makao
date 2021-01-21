#ifndef SCREEN
#define SCREEN

#include <Windows.h>
#include <vector>
#include "../shared/Card.h"
#include <iostream>
#include <io.h> //for _setmode
#include <fcntl.h> //for _O_U16TEXT
#include <string>

#define screen GetScreen()

#define SPADE L"\u2660"
#define CLUB L"\u2663"
#define HEART L"\u2665"
#define DIAMOND L"\u2666"

#define PRINT_SUIT(x)	_setmode(_fileno(stdout), _O_U16TEXT); \
						wcout << x; \
						_setmode(_fileno(stdout), _O_TEXT);

struct lobbylist {
	int l1;
	int l2;
	int l3;
	int l4;
	int l5;
	int l6;
	int l7;
	int l8;
	int l9;
};

struct windowsize {
	int x;
	int y;
};

class Screen {
	friend class Card;
public:
	Screen();
public:
	void DisplaySplashScreen();
	void DisplayMainMenu();
	void DisplayLobby(std::string datain);
	lobbylist DisplayLobbyList(std::string datain);
	void DisplayWaitScreen();
	void DisplayGameOverScreen(std::string datain);
	void ClearScreen();

	void DisplayGameScreen(std::string datain);

private:
	static COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
	static void setCursorPosition(int x, int y);
	windowsize GetWindowSize();
	void Fill(bool line);
	/*
	   Color             Background Foreground
    ---------------------------------------------
    Black            0           0
    Blue             1           1
    Green            2           2
    Cyan             3           3
    Red              4           4
    Magenta          5           5
    Brown            6           6
    White            7           7
    Gray             -           8
    Intense Blue     -           9
    Intense Green    -           10
    Intense Cyan     -           11
    Intense Red      -           12
    Intense Magenta  -           13
    Yellow           -           14
    Intense White    -           15
	finalcolor = 16*bgcolor + fgcolor
	*/
	void setcolor(int color) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		return;
	}
};

Screen* GetScreen();



#endif