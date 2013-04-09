#include "server_msg_handler.h"
#include "server.h"
#include "protocol.h"
#include "database.h"

#ifdef AMNESIA_DB
#include "memdb.h"
#else
#include "filedb.h"
#endif

#include <iostream>

using namespace std;
using namespace client_server;
using namespace protocol;

void error(Server &server, Connection *conn, const string &msg)
{
	server.deregisterConnection(conn);
	delete conn;
	cerr << msg << endl;
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

#ifdef AMNESIA_DB
	mem_database db;
#else
	file_database db;
#endif

	while (true) {
		Connection* conn = server.waitForActivity();

		if (conn != 0) {
			try {
				server_msg_handler handler(conn, db);
				handler.handle();
			} catch (ConnectionClosedException&) {
				error(server, conn, "Client closed the connection.");
			} catch (msg_handler::malformed_req_exception&) {
				error(server, conn, "The request was malformed from the client's"
						" side.");
			}
		} else {
			server.registerConnection(new Connection);
			cout << "New client connects" << endl;
		}
	}

	return 0;
}
