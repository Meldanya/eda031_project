#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "connection.h"
#include "client_msg_handler.h"
#include "ng.h"
#include "art.h"

#define PROMPT "> "

using namespace std;
using namespace client_server;

void print_newsgroups(const vector<ng> &newsgroups)
{
	cout << "Newsgroups:" << endl;
	cout << "===========" << endl;
	for (const ng &ng : newsgroups) {
		cout << ng;
	}

	cout << endl << "To list articles in a newsgroup, execute: "
		"list_art [ng_id] where [ng_id] is the ID of the newsgroup in "
		"the left column above.";
}

void print_articles(const vector<pair<size_t, string>> &articles)
{
	cout << "Articles:" << endl;
	cout << "=========" << endl;
	for (const pair<size_t, string> &art : articles) {
		cout << art.first << ": " << art.second << endl;;
	}

	cout << endl << "To read an article, execute: read_art [ng_id] [art_id] "
		"where [ng_id] is the ID of the newsgroup and [art_id] is the ID of the "
		"article.";
}

void print_article(const art &article)
{
	cout << article.title << endl;
	cout << string(article.title.size(), '=') << endl;

	cout << "By: " << article.author << endl << endl;

	cout << article.content << endl;
}

int main(int argc, const char *argv[])
{
	if (argc != 3) {
		cerr << "Usage: ng_client server-addr port-number" << endl;
		exit(1);
	}

	Connection conn(argv[1], atoi(argv[2]));
	client_msg_handler handler(&conn);

	cout << PROMPT;
	string line;
	while (getline(cin, line)) {
		stringstream ss(line, ios_base::in);
		string cmd;
		ss >> cmd;

		// Save old exception states.
		stringstream::iostate oldstate = ss.exceptions();
		if (cmd == "list_ng") {
			try {
				print_newsgroups(handler.send_list_ng());
			} catch (client_msg_handler::wrong_answer_exception&) {
				cout << "Yup" << endl;
			}
		} else if (cmd == "list_art") {
			ss.exceptions(stringstream::failbit | stringstream::badbit);
			size_t ng_id;
			try {
				ss >> ng_id;
				print_articles(handler.send_list_art(ng_id));
			} catch (stringstream::failure&) {
				cerr << "FAAAAAIL" << endl;
			} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
				cerr << "No such newsgroup ID. Use 'list_ng' to see available "
					"newsgroups." << endl;
			}
		} else if (cmd == "read_art") {
			ss.exceptions(stringstream::failbit | stringstream::badbit);
			size_t ng_id;
			size_t art_id;
			try {
				ss >> ng_id;
				ss >> art_id;
				print_article(handler.send_get_article(ng_id, art_id));
			} catch (stringstream::failure&) {
				cerr << "FAAAAAIL" << endl;
			} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
				cerr << "No such newsgroup ID. Use 'list_ng' to see available "
					"newsgroups." << endl;
			} catch (client_msg_handler::article_does_not_exist_exception&) {
				cerr << "No such article ID. Use 'list_art " << ng_id << "' to "
					"see available articles in this newsgroup." << endl;
			}
		} else if (cmd == "create_ng") {
			ss.exceptions(stringstream::failbit | stringstream::badbit);
			string ng_name;
			try {
				ss >> ng_name;
				handler.send_create_ng(ng_name);
				cout << "Newsgroup created!" << endl;
			} catch (stringstream::failure&) {
				cerr << "FAAAAAIL" << endl;
			} catch (client_msg_handler::newsgroup_already_exists_exception&) {
				cerr << "A newsgroup of that name already exist, please choose "
					"another one." << endl;
			}
		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else if (cmd == "list_art") {

		} else {
			cerr << "Unknown command, use 'help' to list available commands."
				<< endl;
		}

		ss.exceptions(oldstate);

		cout << endl << PROMPT;
	}

	return 0;
}
