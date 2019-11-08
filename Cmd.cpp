#include <string.h>
#include "Cmd.h"
#include "Login.h"
#include <stdbool.h>
#include <vector>
#include <sstream> 
#include <fstream>

using namespace std;

#define BUFFER_LENGTH 1000
#define TABLE_CAP 100

TABLE* fTables = (TABLE*)calloc(20, sizeof(TABLE));

Cmd::Cmd() {

}

void Cmd::initialize(TABLE* table) {

	table->name = "";
	table->colCount = 0;
	table->colCap = TABLE_CAP;
	table->col = (COLUMN*) calloc(TABLE_CAP, sizeof(COLUMN));
	
	for (int i = 0; i < TABLE_CAP;i++) {
		table->col[i].rows = (string*)calloc(TABLE_CAP, sizeof(string));
		table->col[i].rowCap = TABLE_CAP;
	}
}

void Cmd::freeTable(TABLE* table) {
	free(table->col);
	table->col = NULL;
	table->name = "";
	table->colCount = 0;
	table->colCap = 0;

	for (int i = 0; i < table->col[i].rowCount; i++) {
		table->col[i].rowCap = 0;
		table->col[i].rows = NULL;
	}
}

bool Cmd::isFreeSpace(TABLE* table) {
	bool riadky = false;
	for (int j = 0; j < table->colCount; j++) {
		if (table->col[j].rowCount >= table->col[j].rowCap) {
			riadky = true;
		}
	}

	if (table->colCount >= table->colCap || riadky) {
		COLUMN* pomA = (COLUMN*) realloc(table->col, 2 * sizeof(COLUMN) * table->colCap);
		if (pomA == NULL) {
			fprintf(stderr, "Reallocate error Column.\n");
			return false;
		}
		table->col = pomA;
		table->colCap = 2 * table->colCap;
		
		for (int j = 0; j < table->colCap; j++) {
			string* pomB = (string*)realloc(table->col[j].rows, 2 * sizeof(string) * table->col[j].rowCap);
			if (pomB == NULL) {
				fprintf(stderr, "Reallocate error Row.\n");
				return false;
			}
			table->col[j].rows = pomB;
			table->col[j].rowCap *= 2;
		}
	}
	return true;
}

string Cmd::EnumToStrTp(type tp) {
	switch (tp) {
	case INT:
		return "INT";
		break;
	case DOUBLE:
		return "DOUBLE";
		break;
	case STRING:
		return "STRING";
		break;
	case BOOLEAN:
		return "BOOLEAN";
		break;
	case UNDEF:
		return "UNDEF";
		break;
	}
}

string Cmd::EnumToStrPr(prikazy p) {
	switch (p) {
	case CREATE:
		return "CREATE";
		break;
	case ADD:
		return "ADD";
		break;
	case DELETE:
		return "DELETE";
		break;
	case LOGIN:
		return "LOGIN";
		break;
	case REGISTER:
		return "REGISTER";
		break;
	case PRINT:
		return "PRINT";
		break;
	case SAVE:
		return "SAVE";
		break;
	case LOAD:
		return "LOAD";
		break;
	}
}

enum prikazy Cmd::strToEnumPr(string str) {
	char* ss = new char[TABLE_CAP];
	Login* l = new Login();
	strcpy(ss, str.c_str());
	l->toUpperStr(ss);
	if (!strcmp(ss, "CREATE")) { return CREATE; }
	if (!strcmp(ss, "ADD")) { return ADD; }
	if (!strcmp(ss, "DELETE")) { return DELETE; }
	if (!strcmp(ss, "LOGIN")) { return LOGIN; }
	if (!strcmp(ss, "REGISTER")) { return REGISTER; }
	if (!strcmp(ss, "PRINT")) { return PRINT; }
	if (!strcmp(ss, "SAVE")) { return SAVE; }
	if (!strcmp(ss, "LOAD")) { return LOAD; }
}

string Cmd::printEnumPr() {
	string out = "\n-------------------------\n";
	prikazy p[8] = {CREATE, ADD, DELETE, LOGIN, REGISTER, PRINT, SAVE, LOAD};
	for (int i = 0; i < 8; i++) {
		out += EnumToStrPr(p[i]) + "\n";
	}
	out += "-------------------------";
	return out;
}

enum type Cmd::strToEnumType(string str) {
	char* ss = new char[TABLE_CAP];
	Login* l = new Login();
	strcpy(ss,str.c_str());
	l->toUpperStr(ss);
	if (!strcmp(ss, "INT")) { return INT; }
	if (!strcmp(ss, "DOUBLE")) { return DOUBLE; }
	if (!strcmp(ss, "STRING")) { return STRING; }
	if (!strcmp(ss, "BOOLEAN")) { return BOOLEAN; }
	else { return UNDEF; }
}

vector<string> Cmd::split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

TABLE* Cmd::createTable(string cmd) {

	TABLE* t = new TABLE();
	Login* l = new Login();
	char* name = new char[TABLE_CAP];
	type type;

	Cmd::initialize(t);
	vector<string> str = split(cmd, ' ');
	int i = 0;
	int znak = 0;
	int count = str.size();

	t->name = str[0];
	for (i = 1; i < count; i++) {
		vector<string> pars = split(str[i], ':');
		type = strToEnumType(pars[0]);
		strcpy(t->col[i - 1].name, pars[1].c_str());
		t->col[i - 1].type = type;
		//printf("|	%d:%s	 |", t->col[i - 1].type, t->col[i-1].name);
		t->colCount++;
		//create tab INT:rok STRING:meno DOUBLE:dd ASF:KOKOOO
	}

	for (int i = 0; i < 20; i++) {
		if (fTables[i].name == "") {
			fTables[i].name = t->name;
			fTables[i] = *t;
			break;
		}
	}

	return t;
}

string headline(TABLE* t) {
	string ciara;
	ciara += '\n';
	for (int i = 0; i < t->colCount; i++) {
		ciara += "-------------------------";
	}
	ciara += '\n';
	return ciara;
}

string Cmd::printColumns(TABLE* t) {
	string str;
	string ciara = headline(t);
	char pom[BUFFER_LENGTH] = {};
	sprintf(pom, "| %10s: %-9s |","TABLE",t->name.c_str());
	str += "\n-------------------------\n" +string(pom)+ciara;
	for (int i = 0; i < t->colCount; i++) {
		sprintf(pom,"| %10s:%-10s |",EnumToStrTp(t->col[i].type).c_str(),t->col[i].name);
		str += string(pom);
	}
	str += ciara;
	return str;
}

string printRows(TABLE* t) {
	string str;
	string ciara = headline(t);
	char pom[BUFFER_LENGTH] = {};
	strcpy(pom,t->name.c_str());
	if (t->col != NULL) {
		for (int i = 0; i < t->col[0].rowCount; i++) {
			for (int j = 0; j < t->colCount; j++) {
				sprintf(pom, "| %21s |", t->col[j].rows[i].c_str());
				str += string(pom);

				if (j == t->colCount - 1 && i < t->col[0].rowCount-1) {
					str += "\n";
				}
			}

		}
		str += ciara;
		return str;
	}
	else {
		return "NOT VALID TABLE!";
	}
}

char* Cmd::loginUsr(char* cmd) {
	Login* l = new Login();
	User* userPom;
	char* out = new char[BUFFER_LENGTH / 10];

	vector<string> str = split(cmd,' ');

	char* meno = new char[TABLE_CAP];
	char* heslo = new char[TABLE_CAP];

	if (str.size() < 2) {
		sprintf(out, "\nYou should use: login <username> <password>\n");
		return out;
	}

	strcpy(meno, str[0].c_str() + '\0');
	strcpy(heslo,str[1].c_str() + '\0');

	userPom = new User(l->toUpperStr(meno), heslo, false);
	if (l->findUser(userPom)->login) {
		sprintf(out, "\n\nUser>  %s  ALREADY LOGGED IN!\n\n", meno);
		return out;
	}
	else if (l->tryAddUser(userPom)==0) {
		sprintf(out, "\n\nUser>  %s  LOGGED IN SUCCESSFULLY!\n\n", meno);
		return out;
	}
	else if(l->tryAddUser(userPom)==1){
		sprintf(out, "\n\nBad username or password!\n\n");
		return out;
	}
	else {
		sprintf(out, "\n\nUser %s is not yet registered!\nUse-> register <username> <password>\n\n", meno);
		return out;
	}
}

char* Cmd::registerUsr(char* cmd) {
	Login* l = new Login();
	User* userPom;
	char* out = new char[BUFFER_LENGTH / 10];

	vector<string> str = split(cmd, ' ');

	if (str.size() < 2) {
		sprintf(out, "\n\nBad username or password!\n\n");
		return out;
	}

	char* meno = new char[TABLE_CAP];
	char* heslo = new char[TABLE_CAP];

	strcpy(meno, str[0].c_str() + '\0');
	strcpy(heslo, str[1].c_str() + '\0');

	userPom = new User(l->toUpperStr(meno), heslo, false);
	if (l->tryAddUser(userPom)==1) {
		sprintf(out, "\n\nUser>  %s  ALREADY EXISTS!\n\n", meno);
		return out;
	}
	else if (l->addUser(userPom)) {

		sprintf(out, "\n\nUser>  %s  SUCCESSFULLY REGISTERED!\n\n", meno);
		saveUsers();
		return out;
	}
	else {
		sprintf(out, "\n\n!!! INCORRECT SYNTAX !!!\n\n");
		return out;
	}
}

void Cmd::saveUsers() {
	Login* l = new Login();
	remove("users.txt");
	FILE* output = fopen("D:\\Semestralka UDSP\\ServerCpp\\users.txt", "w+");
	for (int i = 0; i < l->userCount; i++) {
		fprintf( output,"%s;%s;\n",l->fUsers[i].username.c_str(),l->fUsers[i].password.c_str() );
	}
	fclose(output);
}

TABLE* Cmd::findTable(string str) {
	for (int i = 0; i < 20;i++) {
		if (fTables[i].name == str) {
			return &fTables[i];
		}
	}
	return new TABLE();
}

string Cmd::saveTable(string str) {
	string cesta = "D:\\Semestralka UDSP\\ServerCpp\\";
	char* out = new char[TABLE_CAP];
	Login* l = new Login();
	TABLE* t = new TABLE();

	t = findTable(l->trim(str));
	if (t->name == "") {
		sprintf(out,"\nTABLE: <%s> NOT SAVED...\n", t->name.c_str());
		return out;
	}
	str = t->name + ".txt";
	cesta += str;
	remove(str.c_str());
	FILE* output = fopen(cesta.c_str(), "w+");

	for (int i = 0; i < t->colCount; i++) {
		fprintf(output, "%s:%-s ", EnumToStrTp(t->col[i].type).c_str(), t->col[i].name);
	}
	fprintf(output, ";\n");
	for (int i = 0; i < t->col[0].rowCount; i++) {
		for (int j = 0; j < t->colCount; j++) {
			fprintf(output,"%s* ",t->col[j].rows[i].c_str());

			if (j == t->colCount - 1 && i < t->col[0].rowCount - 1) {
				fprintf(output, ";\n");
			}
			else if (i == t->col[0].rowCount - 1 && j == t->colCount - 1){
				fprintf(output, ";");
			}
		}

	}
	fclose(output);
	sprintf(out,"\nTABLE: <%s> SAVED SUCCESSFULLY !!!\n", t->name.c_str());
	return out;
}

bool Cmd::addRow(TABLE* t, string str) {
	vector<string> strVs;
	Login* l = new Login();
	int sizeVs = 0;
	int count;

		strVs = split(str, '*');
		sizeVs = strVs.size() - 1;
		if (sizeVs < 1 || sizeVs < t->colCount || sizeVs > t->colCount) {
			return false;
		}

		for (int j = 0; j < sizeVs; j++) {
			count = ++t->col[j].rowCount;
			if (isFreeSpace(t)) {
				t->col[j].rows[count-1] = l->trim(strVs[j].c_str());
			}
		}

	return true;
}

string Cmd::checkTypes(TABLE* t, string str) {
	vector<string> strVs;
	int sizeVs = 0;
	strVs = split(str,' ');
	sizeVs = strVs.size();
	str = "";
	for (int i = 0; i < sizeVs; i++) {
		strVs[i] += "* ";
		str += strVs[i].c_str();
	}

	return str;
}

string Cmd::loadTable(string str) {
	string cesta = "D:\\Semestralka UDSP\\ServerCpp\\";
	Login* l = new Login();
	TABLE* t = new TABLE();
	char* pom = new char[TABLE_CAP];
	char* out = new char[TABLE_CAP];
	vector<string> strV;
	vector<string> strVs;
	t->name = l->trim(str);
	str = t->name + ".txt";
	cesta += str;

	std::ifstream output(cesta);
	std::string content((std::istreambuf_iterator<char>(output)),
						(std::istreambuf_iterator<char>()));

	strV = split(content,';');
	int sizeV = strV.size();
	int sizeVs = 0;
	t = createTable(t->name+' '+strV[0]);

	for (int i = 1; i < sizeV; i++) {
		addRow(t, strV[i]);
	}

	sprintf(out, "\nTABLE: <%s> LOADED SUCCESSFULLY !!!\n", t->name.c_str());
	return out;
}

string Cmd::Command(char* str) {
	char* par1 = new char[BUFFER_LENGTH / 10];
	char* par2 = new char[BUFFER_LENGTH / 10];
	char* pom1 = strchr(str, ' ')+1;
	char* pom2;
	char* out = new char[BUFFER_LENGTH / 10];
	Login* l = new Login();
	TABLE* t = new TABLE();
	
	strncpy(par1, str, pom1 - str);
	par1[pom1 - str] = '\0';
	l->toUpperStr(par1);

	if (string(par1).find('?') != std::string::npos) {
		return printEnumPr();
	}
	
	switch (strToEnumPr(l->trim(par1))) {
	case 1:

		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\ncreate <table_name> <col_type>:<col_name> ...\n");
			return out;
		}
		return printColumns(createTable(pom1));
		break;

	case 2: 

		pom2 = strchr(pom1,' ')+1;
		strncpy(par2, pom1, pom2 - pom1);
		par2[pom2 - pom1]= '\0';


		t = findTable(l->trim(par2));
		if (addRow(t, checkTypes(t, pom2))) {

			sprintf(out, "\nRecord ADDED SUCCESSFULLY !!!\n");
			return out;
		}
		else {
			sprintf(out, "\nRecord DENIED ...\n");
			return out;
		}
		break;

	case 3: 
		
		return "delete";
		break;

	case 4: 

		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\nlogin <username> <password>\n");
			return out;
		}
		return loginUsr(pom1);
		break;

	case 5: 
		
		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\nregister <username> <password>\n");
			return out;
		}
		return registerUsr(pom1);
		break;

	case 6:

		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\nprint <table_name>\n");
			return out;
		}
		t = findTable(l->trim(pom1));
		return printColumns(t) + printRows(t);
		break;

	case 7:

		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\nsave <table_name>\n");
			return out;
		}
		return saveTable(pom1);
		break;

	case 8:

		if (string(pom1).find('?') != std::string::npos || !strcmp(pom1, "")) {
			sprintf(out, "\nload <table_name>\n");
			return out;
		}
		return loadTable(pom1);
		break;

	default: 

		return "\n!!! Unknown command !!!"+printEnumPr();
		break;
	}
}
