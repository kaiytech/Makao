// UMMA-Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

#include "SocketHandler.h"
#include "dbAccess.h"
#include "SessionHandler.h"

int main(int argc, char* argv[])
{
    std::cout << "========================\n";
    std::cout << "==   Makao - Server   ==\n";
    std::cout << "========================\n";
    std::cout << "=== Kajetan Krawczyk ===\n";
    std::cout << "==== Michal Radtke =====\n";
    std::cout << "========================\n";


    /*
    SessionHandler *sh = new SessionHandler();
    sh->BeginGame();
    sh->BeginGame();
    sh->BeginGame();

    Game* g = sh->GetGame(1);
    if (g == NULL) cout << "null";
    cout << g->GetId();

    system("pause");
    return 0;
    */


    string errmsg = "";
    if (!GetDb()->isConnected(errmsg)) {
        cout << "\n\tError connecting to the database:";
        cout << "\n\t" << errmsg << endl;
        system("pause");
        return 0;
    }


    Handle();

    return 0;
}
