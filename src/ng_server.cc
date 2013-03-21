#include "server.h"

#include "msg_handler.h"

#include <iostream>

using namespace std;
using namespace client_server;

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		cerr << "Usage: ng_server port-number" << endl;
		exit(1);
	}

	Server server(atoi(argv[1]));

	if (!server.isReady()) {
		cerr << "Server initialization error" << endl;
		exit(1);
	}

	while (true) {
		Connection* conn = server.waitForActivity();

		if (conn != 0) {
			try {
				msg_handler handler(conn);
				unsigned char cmd = handler.read_cmd();
				cout << hex << "0x" << (unsigned int) cmd << endl;
			} catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				delete conn;
				cout << "Client closed connection" << endl;
			}
		} else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
	}

	return 0;
}
