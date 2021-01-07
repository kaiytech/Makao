#ifndef GAME
#define GAME

#define game GetGame()

class Game {
public:
	Game();
	bool IsInLobby();
	bool IsInGame();
	bool IsInLobbyList();
	bool IsInGameOver();

	void SetInLobby(bool state);
	void SetInGame(bool state);
	void SetInLobbyList(bool state);
	void SetInGameOver(bool state);

	void AssignId(int id);
	bool HasId();
	int GetId();

private:
	bool bInLobby;
	bool bInGame;
	bool bInLobbyList;
	bool bInGameOver;
	int iId;
};

Game* GetGame();

#endif