// UMMA-Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include <iostream>
#include "CSocketHandler.h"
#include "CScreen.h"
#include <conio.h>

int main()
{
    //screen->DisplaySplashScreen();
    //_getch();
    screen->DisplayWaitScreen();

    Handle();
    system("pause");
}
