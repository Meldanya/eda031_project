#include "msg_handler.h"
#include "protocol.h"
#include "database.h"

#include <iostream>

using namespace client_server;
using namespace protocol;
using namespace std;

int msg_handler::read_int()
{
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();

    return (byte1 << 24) | (byte2 << 16) |
        (byte3 << 8) | byte4;
}

void msg_handler::write_int(int value)
{
    conn->write((value >> 24) & 0xFF);
    conn->write((value >> 16) & 0xFF);
    conn->write((value >> 8)  & 0xFF);
    conn->write(value & 0xFF);
}

int msg_handler::read_parameter_int()
{
	if (conn->read() != Protocol::PAR_NUM)
		throw malformed_req_exception();

	return read_int();
}

void msg_handler::write_parameter_int(int value)
{
	conn->write(Protocol::PAR_NUM);
	write_int(value);
}

string msg_handler::read_parameter_string()
{
	if (conn->read() != Protocol::PAR_STRING)
		throw malformed_req_exception();

	int num_bytes = read_int();
	string name;
	while (num_bytes--)
		name += conn->read();

    return name;
}

void msg_handler::write_string(const string &s)
{
	conn->write(Protocol::PAR_STRING);
	write_int(s.length());
	for (auto c : s)
		conn->write(c);
}

void msg_handler::read_end()
{
	unsigned char end = conn->read();
	if (end != Protocol::COM_END)
		throw malformed_req_exception();
}
