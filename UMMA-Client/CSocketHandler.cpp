﻿#include "CSocketHandler.h"
#include <windows.h>
#include <WinUser.h>
#include <tchar.h>
#include <chrono>
#include "CGame.h"
#include "CScreen.h"

#define INFO_BUFFER_SIZE MAX_COMPUTERNAME_LENGTH + 1
#define SERVER_IP "192.168.1.77"
#define DEBUG_LOG false
#define KEYPRESSED(x) (GetConsoleWindow() == GetForegroundWindow()) && GetKeyState(x) && GetAsyncKeyState(x)

#define WAIT_TIME 500

using namespace std;

void Handle() {

	long SUCCESSFUL;
	WSAData WinSockData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 1);
	SUCCESSFUL = WSAStartup(DLLVersion, &WinSockData);

	string RESPONSE;
	string CONVERTER;
	char message[200];

	SOCKADDR_IN ADDRESS;

	ADDRESS.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	ADDRESS.sin_family = AF_INET;
	ADDRESS.sin_port = htons(444);

	int uid = -1;
	int roomid = -1;
	lobbylist l;
	l.l1 = -1; l.l2 = -1; l.l3 = -3; l.l4 = -1; l.l5 = -1; l.l6 = -1; l.l7 = -1; l.l8 = -1; l.l9 = -1;

	bool bAskForStatus = false;

	while (true) {
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, NULL);
	int i = 1;
	char msgtosend[256] = "";
		while (true)
		{
			// if it's stupid but it works then it ain't stupid
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds> (
				std::chrono::system_clock::now().time_since_epoch()
				);
			long long int time_target = ms.count() + WAIT_TIME;

			if(DEBUG_LOG) cout << "Input time.";
			while (true) {
				if (bAskForStatus) {
					bAskForStatus = false;
					sprintf_s(msgtosend, "status|%i", game->GetId());
					break;
				}
				std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds> (
					std::chrono::system_clock::now().time_since_epoch()
					);
				long long int time_now = ms.count() + 0;
				if (time_now > time_target) {

					if (!game->HasId()) {
						sprintf_s(msgtosend, "status");
					}
					else
						sprintf_s(msgtosend, "status|%i", game->GetId());

					if (game->IsInLobbyList()) {
						sprintf_s(msgtosend, "lobbylist|");
					}

					break;
				}
				// Main menu:
				if (game->HasId() && !game->IsInGame() && !game->IsInLobby() && !game->IsInLobbyList()) {
					if (KEYPRESSED(0x31)) { // 1
						sprintf_s(msgtosend, "createlobby|%i", game->GetId());
						game->SetInLobby(true);
						break;
					}
					if (KEYPRESSED(0x32)) { // 2
						//cout << "ID pokoju: ";
						//cin >> roomid;
						sprintf_s(msgtosend, "lobbylist|");
						game->SetInLobbyList(true);
						break;
					}
					if (KEYPRESSED(0x33)) { // 3
						return; // end
					}
				}

				// Lobby:
				if (game->HasId() && game->IsInLobby()) {
					if (KEYPRESSED(0x31)) { // 1
						sprintf_s(msgtosend, "begingame|%i", game->GetId());
						//game->SetInGame(true);
						break;
					}
					if (KEYPRESSED(0x30)) { // 0
						sprintf_s(msgtosend, "leavelobby|%i", game->GetId());
						game->SetInLobby(false);
						break;
					}
				}

				// Lobby list:
				if (game->HasId() && game->IsInLobbyList()) {
					//todo: 1-9
					if (KEYPRESSED(0x31)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l1); break;  } // 1
					if (KEYPRESSED(0x32)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l2); break; } // 2
					if (KEYPRESSED(0x33)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l3); break; } // 3
					if (KEYPRESSED(0x34)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l4); break; } // 4
					if (KEYPRESSED(0x35)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l5); break; } // 5
					if (KEYPRESSED(0x36)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l6); break; } // 6
					if (KEYPRESSED(0x37)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l7); break; } // 7
					if (KEYPRESSED(0x38)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l8); break; } // 8
					if (KEYPRESSED(0x39)) { sprintf_s(msgtosend, "joinlobby|%i|%i", game->GetId(), l.l9); break; } // 9
					if (KEYPRESSED(0x30)) { // 0
						game->SetInLobbyList(false);
						break;
					}
				}

				if (game->HasId() && game->IsInGame()) {
					if (KEYPRESSED(0x31)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "1"); break; } // play card 1
					if (KEYPRESSED(0x32)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "2"); break; } // play card 2
					if (KEYPRESSED(0x33)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "3"); break; } // play card 3
					if (KEYPRESSED(0x34)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "4"); break; } // play card 4
					if (KEYPRESSED(0x35)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "5"); break; } // play card 5
					if (KEYPRESSED(0x36)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "6"); break; } // play card 6
					if (KEYPRESSED(0x37)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "7"); break; } // play card 7
					if (KEYPRESSED(0x38)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "8"); break; } // play card 8
					if (KEYPRESSED(0x39)) { sprintf_s(msgtosend, "playcard|%i|%s|", game->GetId(), "9"); break; } // play card 9
					if (KEYPRESSED(0x5A)) { sprintf_s(msgtosend, "drawcard|%i", game->GetId()); break; } // draw card (Z)
					if (KEYPRESSED(0x58)) { sprintf_s(msgtosend, "leavegame|%i", game->GetId()); break; } // leave game (X)

				}
			}
			if(DEBUG_LOG) cout << "\nSending input: " << msgtosend << ".";


			if(DEBUG_LOG) cout << "\n\tConnecting to the Makao server (attempt: " << i << ")";
			int connresult = connect(sock, (SOCKADDR*)&ADDRESS, sizeof(ADDRESS));
			if (connresult == SOCKET_ERROR) {
				if (i == 3) {
					if(DEBUG_LOG) cout << "\n\tConnection failed after 3 retries. Quiting." << endl;
					return;
				}
				i++;
				continue;
			}
			struct sockaddr_in* s = (struct sockaddr_in*)&ADDRESS;
			if (DEBUG_LOG) cout << "\n\tConnected to " << inet_ntoa(s->sin_addr) << "." << endl;
			i = 1;
			break;
		}

		char dummymsg[200];
		SUCCESSFUL = recv(sock, dummymsg, sizeof(dummymsg), NULL); //recv a dummy message

		/*
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		size_t ncc;
		GetComputerName(infoBuf, &bufCharCount);
		char computername[INFO_BUFFER_SIZE] = "";
		wcstombs_s(&ncc, computername, infoBuf, INFO_BUFFER_SIZE);
		*/

		SUCCESSFUL = send(sock, msgtosend, sizeof(msgtosend), NULL); //send response
		SUCCESSFUL = recv(sock, message, sizeof(message), NULL);
		CONVERTER = message;

		string st = message;

		if (st.rfind("Hi", 0) == 0) {
			screen->DisplayMainMenu();
		}

		if (st.rfind("AFS", 0) == 0) {
			screen->DisplayWaitScreen();
			game->SetInLobbyList(false);
			bAskForStatus = true;
		}

		// assign new ID
		if ((!game->HasId()) && (st.rfind("newid|", 0) == 0)) {
			int last = st.find("-");
			st = st.substr(0,last);
			st = st.substr(6, st.length());
			game->AssignId(stoi(st));
			screen->DisplayMainMenu();
		}

		// lobby status
		else if (st.rfind("lobbystatus|", 0) == 0) {
			std::string tosend = st.substr(12, st.length());
			game->SetInLobby(true);
			screen->DisplayLobby(tosend);
		}

		// lobby list
		else if (st.rfind("lobbylist|", 0) == 0) {
			std::string tosend = st.substr(10, st.length());
			l = screen->DisplayLobbyList(tosend);
		}

		// game stauts
		else if (st.rfind("gamestatus|", 0) == 0) {
			std::string tosend = st.substr(11, st.length());
			//todo: game actions
			if (!game->IsInGame()) screen->ClearScreen();
			screen->DisplayGameScreen(tosend);
			game->SetInGame(true);
			game->SetInLobby(false);
			game->SetInLobbyList(false);
		}

		else if (st.rfind("endgame|", 0) == 0) {
			game->SetInGame(false);
			game->SetInLobby(false);
			game->SetInLobbyList(false);
			game->SetInGameOver(true);
			screen->DisplayGameOverScreen(st);
		}

		
		if(DEBUG_LOG) cout << "\tMessage from SERVER: \t" << CONVERTER << endl;



		Sleep(100);
	}
	return;
}