#include "msg_handler.h"
#include "protocol.h"

#include <iostream>

using namespace client_server;
using namespace protocol;

unsigned char msg_handler::read_cmd()
	throw(ConnectionClosedException)
{
	return conn->read();
}
