#include "dbAccess.h"

static DbAccess* __g_db = 0;

// Creates an instance of a database controller if needed, and returns it
DbAccess* GetDb() {
	if (!__g_db)
		__g_db = new DbAccess();
	return __g_db;
}

// Constructor
DbAccess::DbAccess() {
	std::fstream fs;
	fs.open("umma.db");

	if (fs.fail()) init(); // init a new db and continue
	fs.close(); // we don't need the fstream anymore.

	if (!isConnected()) connect();
	if (!isConnected()) return;
}

// Update Client Entry
// Updates a client entry, creates one if needed.
bool DbAccess::UpdateClientEntry(std::string ip, std::string name) {
	if ((ip.length() <= 0) || (name.length() <= 0)) return false;

	time_t t = time(NULL);
	std::stringstream ss;
	ss << t;
	std::string st = ss.str();

	int rc = 0; char* emsg = 0;
	//else, continue:
	std::string sql = "";
	sql =	"REPLACE INTO COMPUTERS " \
			"VALUES (NULL,'" + name + "','" + ip + "','" + st + "');";

	rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &emsg);

	if (rc != SQLITE_OK) {
		std::cout << std::endl << "ERROR #1 IN: UpdateClientEntry(): " << emsg << std::endl;
		return false;
	}
	//else, continue:

	return true;
}

// isConnected
// ~returns true if so
// ~returns an sqlite error message
bool DbAccess::isConnected(std::string &ErrorMessage) {
	ErrorMessage = sErrmsg;
	if (!bErr) return true; else return false;
}

bool DbAccess::isConnected() {
	std::string s = "";
	return isConnected(s);
}

// connect to the database
// see isConnected() to verify
void DbAccess::connect() {
	int rc = sqlite3_open("umma.db", &db);
	if (rc) {
		bErr = 1;
		sErrmsg = sqlite3_errmsg(db);
	}
	else {
		bErr = 0;
	}
}

// initialize the database for first time use
void DbAccess::init() {
	std::cout << "\n\tDatabase file nonexistent. Creating one." << std::endl;
	std::fstream fs;
	fs.open("umma.db", std::fstream::out);
	fs.close(); // we don't need fstream anymore.

	if (!isConnected()) connect();
	if (!isConnected()) return;

	const char *sql = "CREATE TABLE COMPUTERS(" \
		"INTEGER PRIMARY KEY," \
		"NAME TEXT UNIQUE," \
		"ADDRESS TEXT NOT NULL," \
		"LASTSEEN TEXT NOT NULL" \
		"); " \
		"CREATE TABLE LOGINS(" \
		"INTEGER PRIMARY KEY," \
		"USERNAME TEXT," \
		"SEEN TEXT," \
		"COMPUTER TEXT" \
		"); ";

	char* e = 0;
	if (sqlite3_exec(db, sql, NULL, 0, &e) != SQLITE_OK) {
		//fail
		std::cout << "\ne: " << e << std::endl;
		sqlite3_free(e);
	}
	else {
		std::cout << "\tDatabase created successfully.";
	}
	return;
}

// Destructor
DbAccess::~DbAccess() {
	sqlite3_close(db); // !
}