#ifndef SOCKET_HANDLER

#pragma once
#pragma comment(lib,"Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsdkver.h>
#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <string>

void Handle();

#define SCK_VERSION2 0x0202

#endif