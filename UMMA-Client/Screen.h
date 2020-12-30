#ifndef SCREEN
#define SCREEN

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

class Screen {
public:
	Screen();
public:
	void ClearScreen();
	void DisplayMainMenu();
	void DisplayLobby(std::string datain);
	lobbylist DisplayLobbyList(std::string datain);
	void DisplayWaitScreen();
};

Screen* GetScreen();



#endif