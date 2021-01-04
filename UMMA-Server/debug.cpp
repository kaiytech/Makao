#include "debug.h"
#include <iostream>
#include <Windows.h>
#include <ctime>

/*
   Color             Background Foreground
---------------------------------------------
Black            0           0
Blue             1           1
Green            2           2
Cyan             3           3
Red              4           4
Magenta          5           5
Brown            6           6
White            7           7
Gray             -           8
Intense Blue     -           9
Intense Green    -           10
Intense Cyan     -           11
Intense Red      -           12
Intense Magenta  -           13
Yellow           -           14
Intense White    -           15
finalcolor = 16*bgcolor + fgcolor
*/

void setcolor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	return;
}

#define RED     "\x1B[31m"
#define YELLOW  "\x1B[33m"
#define RESET   "\x1B[0m"


void timestamp() {
	std::time_t t = std::time(0);

	// _CRT_SECURE_NO_WARNINGS
	std::tm* now = std::localtime(&t);
	std::string add0, add1, add2, add3, add4 = "";
	if (now->tm_sec + 1 < 10) add0 = "0";
	if (now->tm_min + 1 < 10) add1 = "0";
	if (now->tm_hour + 1 < 10) add2 = "0";
	if (now->tm_mday < 10) add3 = "0";
	if (now->tm_mon + 1 < 10) add4 = "0";
	
	std::ostringstream ss;
	ss << "[" << (now->tm_year + 1900) << "-" << add4 << (now->tm_mon + 1) << "-" << add3 << (now->tm_mday) << " "
		<< add2 << (now->tm_hour) << ":" << add1 << (now->tm_min) << ":" << add0 << (now->tm_sec + 1) << "] ";

	setcolor(8);
	printf(ss.str().c_str());
	setcolor(7);
}



void Console_Message(std::string s) {
	timestamp();
	printf("[INFO]  %s\n", s.c_str());
}

void Console_Warning(std::string s) {
	timestamp();
	setcolor(14);
	printf("[WARN]  %s\n", s.c_str());
	setcolor(7);
}

void Console_Error(std::string s) {
	timestamp();
	setcolor(4);
	printf("[ERR]   %s\n", s.c_str());
	setcolor(7);
}

void Console_Success(std::string s) {
	timestamp();
	setcolor(2);
	printf("[SUCC]  %s\n", s.c_str());
	setcolor(7);
}



void Console_Message(std::ostringstream &s) {
	Console_Message(s.str());
	s.clear();
}

void Console_Warning(std::ostringstream &s) {
	Console_Warning(s.str());
	s.clear();
}

void Console_Error(std::ostringstream &s) {
	Console_Error(s.str());
	s.clear();
}

void Console_Success(std::ostringstream& s) {
	Console_Success(s.str());
	s.clear();
}