#include "User.h"

#include <string>

using std::string;

User::User(string usr, string pass, bool log ) {
	username = usr;
	password = pass;
	login = log;
}


