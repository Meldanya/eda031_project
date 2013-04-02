#include "client_msg_handler.h"
#include "protocol.h"

using namespace std;
using namespace protocol;

vector<ng> client_msg_handler::send_list_ng()
{
	// Send request.
	conn->write(Protocol::COM_LIST_NG);
	conn->write(Protocol::COM_END);

	// Check that answer is correct.
	if (conn->read() != Protocol::ANS_LIST_NG)
		throw wrong_answer_exception();

	// Read the number of newsgroups.
	int num_ng = read_parameter_int();

	// Read all the newsgroups into a vector.
	vector<ng> newsgroups;
	for (int i = 0; i < num_ng; i++) {
		int ng_id = read_parameter_int();
		string ng_name = read_parameter_string();

		ng ng(ng_id, ng_name);
		newsgroups.push_back(ng);
	}

	conn->read(); // Get rid of ANS_END.

	return newsgroups;
}

void client_msg_handler::send_create_ng(const string &newsgroup_name)
{
	// Send request.
	conn->write(Protocol::COM_CREATE_NG);
	write_string(newsgroup_name);
	conn->write(Protocol::COM_END);

	// Check that answer is correct.
	if (conn->read() != Protocol::ANS_CREATE_NG)
		throw wrong_answer_exception();

	if (conn->read() == Protocol::ANS_NAK) {
		unsigned char c = conn->read();
		conn->read(); // Get rid of ANS_END.

		if (c == Protocol::ERR_NG_ALREADY_EXISTS) {
			throw newsgroup_already_exists_exception();
		} else {
			throw wrong_answer_exception();
		}
	}

	conn->read(); // Get rid of ANS_END.
}

void client_msg_handler::send_delete_ng()
{

}

vector<pair<size_t,string>> client_msg_handler::send_list_art(int newsgroup_id)
{
	// Send request.
	conn->write(Protocol::COM_LIST_ART);
	write_parameter_int(newsgroup_id);
	conn->write(Protocol::COM_END);

	// Check that answer is correct.
	if (conn->read() != Protocol::ANS_LIST_ART)
		throw wrong_answer_exception();

	vector<pair<size_t,string>> articles;
	if (conn->read() == Protocol::ANS_ACK) {
		// Read the number of newsgroups.
		int num_art = read_parameter_int();

		// Read all the newsgroups into a vector.
		for (int i = 0; i < num_art; i++) {
			int id = read_parameter_int();
			string name = read_parameter_string();

			articles.push_back(make_pair(id, name));
		}

		conn->read(); // Get rid of ANS_END.
	} else {
		conn->read(); // Get rid of ERR_NG_DOES_NOT_EXIST.
		conn->read(); // Get rid of ANS_END.
		throw newsgroup_does_not_exist_exception();
	}

	return articles;
}

void client_msg_handler::send_create_art()
{

}

void client_msg_handler::send_delete_art()
{

}

art client_msg_handler::send_get_article(size_t newsgroup_id, size_t art_id)
{
	// Send request.
	conn->write(Protocol::COM_GET_ART);
	write_parameter_int(newsgroup_id);
	write_parameter_int(art_id);
	conn->write(Protocol::COM_END);

	// Check that answer is correct.
	if (conn->read() != Protocol::ANS_GET_ART)
		throw wrong_answer_exception();

	art article;
	if (conn->read() == Protocol::ANS_ACK) {
		// Read the number of newsgroups.
		article.title = read_parameter_string();
		article.author = read_parameter_string();
		article.content = read_parameter_string();

		conn->read(); // Get rid of ANS_END.
	} else {
		unsigned char c = conn->read();
		conn->read(); // Get rid of ANS_END.

		if (c == Protocol::ERR_NG_DOES_NOT_EXIST) {
			throw newsgroup_does_not_exist_exception();
		} else if (c == Protocol::ERR_ART_DOES_NOT_EXIST) {
			throw article_does_not_exist_exception();
		}
	}

	return article;
}

