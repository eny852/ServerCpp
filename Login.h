#include <string>
#include "User.h"
#include <stdio.h>

using std::string;
using std::FILE;

class Login {
public:
	User user;
	static int userCount;
	Login();
	static User fUsers[20];
	int tryAddUser(User* usr);
	bool addUser(User* usr);
	void nacitajUserov(FILE* input);
	char* toUpperStr(char* str); 
	string Login::trim(string str);
	User* findUser(User* usr);
};

