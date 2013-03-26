#include "msg_handler.h"
#include "protocol.h"

#include <iostream>

using namespace client_server;
using namespace protocol;
using namespace std;

void msg_handler::handle()
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
			cout << "got list article req." << endl;
			handle_list_art();
			break;
		case Protocol::COM_CREATE_ART:
			cout << "got create article req." << endl;
			handle_create_art();
			break;
		case Protocol::COM_DELETE_ART:
			cout << "got delete article req." << endl;
			handle_delete_art();
			break;
		case Protocol::COM_GET_ART:
			cout << "got get article req." << endl;
			handle_get_article();
			break;
	}
	read_end();
}

void msg_handler::handle_list_ng()
{
	conn->write(Protocol::ANS_LIST_NG);
	write_parameter_int(1);
	write_parameter_int(1);
	write_string("first");
	conn->write(Protocol::ANS_END);
	cout << "Got list req. TODO: handle it" << endl;
}

void msg_handler::handle_create_ng()
{
	string name = read_parameter_string();
	cout << "Got create ng req(" << name << "). TODO: handle it" << endl;
}

void msg_handler::handle_delete_ng()
{
	int ng_id = read_parameter_int();
	cout << "Got delete ng req(" << ng_id << "). TODO: handle it" << endl;
}

void msg_handler::handle_list_art()
{
	int ng_id = read_parameter_int();
	cout << "Got list art. for ng " << ng_id << endl;
}

void msg_handler::handle_create_art()
{
	int ng_id     = read_parameter_int();
	string title  = read_parameter_string();
	string author = read_parameter_string();
	string text   = read_parameter_string();
	printf("Creating article in ng %d:\ntitle: %s\nauthor: %s\ntext: %s\n",
		   ng_id, title.c_str(), author.c_str(), text.c_str());
}

void msg_handler::handle_delete_art()
{
	int ng_id  = read_parameter_int();
	int art_id = read_parameter_int();
	cout << "Removing art " << art_id << " from ng " << ng_id << endl;
}

void msg_handler::handle_get_article()
{
	int ng_id  = read_parameter_int();
	int art_id = read_parameter_int();
	cout << "Getting art " << art_id << " from ng " << ng_id << endl;
}

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
//throw(client_server::ConnectionClosedException, malformed_req_exception)
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
