#ifndef SCREEN
#define SCREEN

#include <Windows.h>
#include <vector>

#define screen GetScreen()

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
public:
	Screen();
public:
	void DisplaySplashScreen();
	void DisplayMainMenu();
	void DisplayLobby(std::string datain);
	lobbylist DisplayLobbyList(std::string datain);
	void DisplayWaitScreen();

	void DisplayGameScreen(std::string datain);

private:
	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput);
	void setCursorPosition(int x, int y);
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