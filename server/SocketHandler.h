#ifndef SOCKET_HANDLER
#define SOCKET_HANDLER

#pragma once
#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsdkver.h>
#include <conio.h>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>


void Handle();

#define SCK_VERSION2 0x0202
#endif