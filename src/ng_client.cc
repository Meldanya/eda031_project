#include <iostream>
#include <string>
#include <sstream>

#include "connection.h"

#define DELIM "> "
using namespace std;
using namespace client_server;

int main(int argc, const char *argv[])
{
	if (argc != 3) {
		cerr << "Usage: ng_client server-addr port-number" << endl;
		exit(1);
	}

	Connection conn(argv[1], atoi(argv[2]));

	cout << DELIM;
	string line;
	while (getline(cin, line)) {
		stringstream ss(line, ios_base::in);
		string cmd;
		ss >> cmd;
		if (cmd == "list_ng") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		}

		cout << endl << DELIM;
	}

	return 0;
}
