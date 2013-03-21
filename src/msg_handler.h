#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include "connection.h"

#include <vector>

class msg_handler
{
	public:
		msg_handler(client_server::Connection *conn) : conn(conn) {}

		unsigned char read_cmd() throw(client_server::ConnectionClosedException);

	private:
		client_server::Connection *conn;
		std::vector<unsigned char> buf;
};

#endif

