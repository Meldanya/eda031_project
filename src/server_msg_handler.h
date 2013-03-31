#ifndef SERVER_MSG_HANDLER_H
#define SERVER_MSG_HANDLER_H

#include "msg_handler.h"
#include "database.h"

class server_msg_handler : public msg_handler
{
	public:
		void handle(); //throw(client_server::ConnectionClosedException);

		server_msg_handler(client_server::Connection *conn, database &db)
			: msg_handler(conn), db(db) {}

	private:
		database &db;

		void handle_list_ng(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_create_ng(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_delete_ng(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_list_art(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_create_art(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_delete_art(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
		void handle_get_article(); //throw(client_server::ConnectionClosedException,
				//malformed_req_exception);
};

#endif
