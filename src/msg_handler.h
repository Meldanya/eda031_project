#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include "connection.h"

#include <vector>


class msg_handler
{
	public:
		typedef struct malformed_req_exception {} malformed_req_exception;

		msg_handler(client_server::Connection *conn) : conn(conn) {}

		void handle() throw(client_server::ConnectionClosedException);

	private:
		client_server::Connection *conn;
		std::vector<unsigned char> buf;

		void handle_list_ng() throw(client_server::ConnectionClosedException,
				malformed_req_exception);
		void handle_create_ng() throw(client_server::ConnectionClosedException,
				malformed_req_exception);
		void handle_delete_ng() throw(client_server::ConnectionClosedException,
				malformed_req_exception);

		bool end() throw(client_server::ConnectionClosedException);
		int read_int() throw(client_server::ConnectionClosedException);
		std::string read_string() throw(client_server::ConnectionClosedException,
				malformed_req_exception);
		int read_parameter_int() throw(client_server::ConnectionClosedException,
				malformed_req_exception);
		void write_int(int value) throw(client_server::ConnectionClosedException);
		void write_string(const std::string &s)
			throw(client_server::ConnectionClosedException,
					malformed_req_exception);
		void write_parameter_int(int value)
			throw(client_server::ConnectionClosedException);
};

#endif

