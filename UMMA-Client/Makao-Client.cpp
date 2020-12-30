// UMMA-Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>
#include "SocketHandler.h"
#include "Screen.h"

int main()
{
    std::cout << "========================\n";
    std::cout << "======== Makao =========\n";
    std::cout << "========================\n";
    std::cout << "=== Kajetan Krawczyk ===\n";
    std::cout << "==== Michal Radtke =====\n";
    std::cout << "========================\n";

    Sleep(1000);
    screen->DisplayWaitScreen();

    Handle();
    system("pause");
}
