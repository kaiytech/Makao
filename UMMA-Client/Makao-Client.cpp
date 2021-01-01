// UMMA-Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>
#include "SocketHandler.h"
#include "Screen.h"

int main()
{
    screen->DisplaySplashScreen();

    Sleep(1000);
    screen->DisplayWaitScreen();

    Handle();
    system("pause");
}
