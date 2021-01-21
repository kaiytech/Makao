#ifndef DEBUG
#define DEBUG
#include <string>
#include <sstream>

#define Msg(x)		{ std::ostringstream os;\
					os << x; \
					Console_Message(os); }
				
#define Warn(x) 	{ std::ostringstream os;\
					os << x; \
					Console_Warning(os); }

#define Error(x)	{ std::ostringstream os;\
					os << x; \
					Console_Error(os); }

#define Success(x)	{ std::ostringstream os;\
					os << x; \
					Console_Success(os); }

void Console_Message(std::string s);
void Console_Warning(std::string s);
void Console_Error(std::string s);
void Console_Success(std::string s);

void Console_Message(std::ostringstream &s);
void Console_Warning(std::ostringstream &s);
void Console_Error(std::ostringstream &s);
void Console_Success(std::ostringstream &s);

//std::ostringstream os;
//os << "[SH] Creating a new game with an ID: " << iNumOfGames;

#endif // !DEBUG
