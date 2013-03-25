#include "msg_handler.h"
#include "server.h"
#include "protocol.h"

#include <iostream>

using namespace std;
using namespace client_server;
using namespace protocol;

void error(Server &server, Connection *conn)
{
	server.deregisterConnection(conn);
	delete conn;
	cout << "Client closed connection" << endl;
}

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
				handler.handle();
			} catch (ConnectionClosedException&) {
				error(server, conn);
			} catch (msg_handler::malformed_req_exception&) {
				error(server, conn);
			}
		} else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
	}

	return 0;
}
