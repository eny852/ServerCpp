#include <string>
#include <stdbool.h>

using std::string;

class User {
public:
	string username;
	string password;
	bool login;
	unsigned int clientNumber;
	User( string usr, string pass, bool log );
	User() = default;
};