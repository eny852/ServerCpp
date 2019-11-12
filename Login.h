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
	int tryAddUser(User* usr, unsigned int cn);
	bool addUser(User* usr);
	void nacitajUserov(FILE* input);
	char* toUpperStr(char* str); 
	string Login::logout(unsigned int cn);
	string Login::trim(string str);
	User* findUser(User* usr,unsigned int cn);
};

