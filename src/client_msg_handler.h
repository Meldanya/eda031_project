#ifndef CLIENT_MSG_HANDLER_H
#define CLIENT_MSG_HANDLER_H

#include "msg_handler.h"

// TODO: Impl. this class
class client_msg_handler : public msg_handler
{
	public:
		client_msg_handler(client_server::Connection *conn)
			: msg_handler(conn) {}
};

#endif
