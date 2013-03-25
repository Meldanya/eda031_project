#include "msg_handler.h"
#include "protocol.h"

#include <iostream>

using namespace client_server;
using namespace protocol;
using namespace std;

void msg_handler::handle()
	throw(ConnectionClosedException)
{
	unsigned char cmd = conn->read();
	switch (cmd) {
		case Protocol::COM_LIST_NG:
			cout << "got list ng req." << endl;
			handle_list_ng();
			break;
		case Protocol::COM_CREATE_NG:
			cout << "got create ng req." << endl;
			handle_create_ng();
			break;
		case Protocol::COM_DELETE_NG:
			cout << "got delete ng req." << endl;
			handle_delete_ng();
			break;
		case Protocol::COM_LIST_ART:
			break;
		case Protocol::COM_CREATE_ART:
			break;
		case Protocol::COM_DELETE_ART:
			break;
		case Protocol::COM_GET_ART:
			break;
	}
}

void msg_handler::handle_list_ng()
	throw(ConnectionClosedException, malformed_req_exception)
{
	if (!end())
		throw malformed_req_exception();

	conn->write(Protocol::ANS_LIST_NG);
	write_parameter_int(1);
	write_parameter_int(1);
	write_string("first");
	conn->write(Protocol::ANS_END);
	cout << "Got list req. TODO: handle it" << endl;
}

void msg_handler::handle_create_ng()
	throw(ConnectionClosedException, malformed_req_exception)
{
	string name = read_string();


	if (!end())
		throw malformed_req_exception();
	cout << "Got create ng req(" << name << "). TODO: handle it" << endl;
}

void msg_handler::handle_delete_ng()
	throw(ConnectionClosedException, malformed_req_exception)
{
	int ng_id = read_parameter_int();

	if (!end())
		throw malformed_req_exception();
	cout << "Got delete ng req(" << ng_id << "). TODO: handle it" << endl;
}

int msg_handler::read_int()
	throw(ConnectionClosedException)
{
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();

    return (byte1 << 24) | (byte2 << 16) |
        (byte3 << 8) | byte4;
}

void msg_handler::write_int(int value)
	throw(ConnectionClosedException)
{
    conn->write((value >> 24) & 0xFF);
    conn->write((value >> 16) & 0xFF);
    conn->write((value >> 8)  & 0xFF);
    conn->write(value & 0xFF);
}

int msg_handler::read_parameter_int()
	throw(ConnectionClosedException, malformed_req_exception)
{
	if (conn->read() != Protocol::PAR_NUM)
		throw malformed_req_exception();

	return read_int();
}

void msg_handler::write_parameter_int(int value)
	throw(ConnectionClosedException)
{
	conn->write(Protocol::PAR_NUM);
	write_int(value);
}

string msg_handler::read_string()
	throw(ConnectionClosedException, malformed_req_exception)
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
	throw(ConnectionClosedException, malformed_req_exception)
{
	conn->write(Protocol::PAR_STRING);
	write_int(s.length());
	for (auto c : s)
		conn->write(c);
}

bool msg_handler::end()
	throw(ConnectionClosedException)
{
	return conn->read() == Protocol::COM_END;
}
