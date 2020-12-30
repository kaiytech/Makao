#ifndef DBACCESS 
#define DBACCESS
#include <winsqlite/winsqlite3.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

class DbAccess {
public:
	DbAccess();
	~DbAccess();

	bool UpdateClientEntry(std::string ip, std::string name);

	bool isConnected(std::string &ErrorMessage);
	bool isConnected();

private:
	void connect();
	void init();

	sqlite3* db;
	
	bool bErr = 1; // 0 - success.
	std::string sErrmsg;
};



DbAccess* GetDb();
#endif