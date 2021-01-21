// UMMA-Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

#include "SocketHandler.h"
#include "SessionHandler.h"
#include "debug.h"

int main(int argc, char* argv[])
{
    Msg("Makao");
    Msg("Copyright (C) 2020-2021");
    Msg("Code: Kajetan Krawczyk");
    Msg("Testing: Michal Radtke");
    Msg(" ");
    Msg("Starting MAKAO SERVER");

    Handle();

    return 0;
}
