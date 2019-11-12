#include <string>
#include "Login.h"
#include <stdbool.h>
#include <iostream>

#define BUFFER_LENGTH 1000

int Login::userCount = 0;
User Login::fUsers[] = {};
using std::string;
using std::FILE;

Login::Login() {

}

User* Login::findUser(User* usr, unsigned int cn) {
	int i;
	for (i = 0; i < userCount; i++) {
		if (fUsers[i].clientNumber == cn) {
			return &fUsers[i];
		}
		if( !strcmp(usr->username.c_str(), fUsers[i].username.c_str()) &&
			!strcmp(usr->password.c_str(), fUsers[i].password.c_str()) &&
			fUsers[i].login == true ) {
			return &fUsers[i];
		}
	}
	return new User(" ", " ", false);
}

 int Login::tryAddUser( User* usr, unsigned int cn ) {

	 if (Login::userCount > 20) {
		 printf("fUsers[20] plne\n");
		 return 0;
	 }

	 int i;
	 for (i = 0; i < userCount; i++) {
		 if (!strcmp(usr->username.c_str(), fUsers[i].username.c_str()) &&
			 !strcmp(usr->password.c_str(), fUsers[i].password.c_str())) {
			 fUsers[i].login = true;
			 fUsers[i].clientNumber = cn;
			 return 0;
		 }
		 if ( !strcmp(usr->username.c_str(),fUsers[i].username.c_str()) &&
			   strcmp(usr->password.c_str(), fUsers[i].password.c_str())) {
			 return 1;
		 }
	 }
	 return 2;
}

 string Login::logout(unsigned int cn) {
	 User* usr = new User();
	 usr = findUser(usr,cn);
	 findUser(usr, cn)->clientNumber = 100;
	 usr->login = false;
	 return "\n" + usr->username + " LOGGED OUT SUCCESSFULLY!\n";
 }

 bool Login::addUser( User* user) {
	 if (tryAddUser(user,100)) {
		 fUsers[Login::userCount++] = *user;
		 return true;
	 }
	 return false;
 }

 char* Login::toUpperStr(char* str) {
	 char* end = str + strlen(str);
	 for (char* i = str; i < end; i++) {
		 *i = toupper(*i);
	 }
	 return str;
 }

 string Login::trim(string str) {
	 string out = "";
	 int i = strlen(str.c_str());
	 for (int j = 0; j < i; j++) {
		 if (!iswspace(str[j])) {
			 out += str[j];
		 }
	 }
	 return out;
 }

 void Login::nacitajUserov(FILE* input) {
	 char* buf = new char[BUFFER_LENGTH];
	 User* user = new User( " ", " ", false );
	 char* usrname = new char[BUFFER_LENGTH/10];
	 char* passw = new char[BUFFER_LENGTH/10];

	 while (fgets(buf, BUFFER_LENGTH - 1, input) != NULL) {
		 char* pom1 = strchr(buf, ';');
		 strncpy(usrname, buf, pom1 - buf);
		 usrname[pom1 - buf] = '\0';
		 toUpperStr(usrname);
		 user->username = usrname;

		 char* pom2 = strchr(pom1 + 1, ';');
		 strncpy(passw, pom1 + 1, pom2 - pom1);
		 passw[pom2 - pom1 - 1] = '\0';
		 user->password = passw;

		 addUser(user);
		 printf("\n--------------\nusername>%s\npassword>%s\n--------------\n", user->username.c_str(), user->password.c_str());
	 }
	 fclose(input);
 }









