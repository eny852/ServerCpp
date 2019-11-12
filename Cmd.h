#include <iostream>
#include <string>
#include <sstream>
#include "SDL_net.h"
#include <vector>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

enum prikazy { CREATE = 1, ADD, DELETE, LOGIN, REGISTER, PRINT, FIND, SAVE, LOAD, SORT, LOGOUT, QUIT };

enum type { INT = 1, DOUBLE, STRING, BOOLEAN, UNDEF };

typedef struct column {
	char name[20];
	type type;
	string* rows;
	int rowCount;
	int rowCap;
}COLUMN;

typedef struct table {
	string name;
	COLUMN* col;
	int colCount;
	int colCap;
}TABLE;

class Cmd {
public: 
	Cmd();
	enum prikazy Cmd::strToEnumPr(string str);
	enum type Cmd::strToEnumType(string str);
	string Cmd::EnumToStrPr(prikazy p);
	string Cmd::EnumToStrTp(type tp);
	string Cmd::printEnumPr();
	string Cmd::Command(char* str, unsigned int clientNumber);
	void Cmd::saveUsers();
	void Cmd::initialize(TABLE* table);
	void Cmd::freeTable(TABLE* table);
	bool Cmd::isFreeSpace(TABLE* table);
	string Cmd::printColumns(TABLE* t);
	char* Cmd::loginUsr(char* cmd, unsigned int cn);
	char* Cmd::registerUsr(char* cmd);
	TABLE* Cmd::findTable(string str);
	string Cmd::loadTable(string str);
	string Cmd::checkTypes(TABLE* t,string str);
	bool Cmd::addRow(TABLE* t, string str);
	string Cmd::sortByCol(string str);
	string Cmd::deletee(string str);
	string Cmd::saveTable(string str);
	string Cmd::findRow(string str);
	vector<string> Cmd::split(string str, char delimiter);
	TABLE* Cmd::createTable(string name);
};


