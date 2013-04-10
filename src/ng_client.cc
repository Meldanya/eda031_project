#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <cstdio>
#include <fstream>

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
	char filename[L_tmpnam];
	if (!std::tmpnam(filename))
		cerr << "Failed to create temporary file." << endl;

	std::ofstream file(filename);
	if (file) {
		file << article.title << endl;
		file << string(article.title.size(), '=') << endl;

		file << "By: " << article.author << endl << endl;

		file << article.content << endl;

		string command = string("$PAGER ") + filename;
		int status = system(command.c_str());
		if (status) {
			cout << article.title << endl;
			cout << string(article.title.size(), '=') << endl;

			cout << "By: " << article.author << endl << endl;

			cout << article.content;
		}

		std::remove(filename);
	}
}

int main(int argc, const char *argv[])
{
	if (argc != 3) {
		cerr << "Usage: ng_client server-addr port-number" << endl;
		exit(1);
	}

	Connection conn(argv[1], atoi(argv[2]));
	client_msg_handler handler(&conn);

	cout << "News client v0.9, 2013-04-10. Use 'help' to see available commands"
		<< endl;
	cout << PROMPT;
	string line;
	while (getline(cin, line)) {
		stringstream ss(line, ios_base::in);

		// Read the command.
		string cmd;
		ss >> cmd;

		// Save old exception states.
		stringstream::iostate oldstate = ss.exceptions();
		ss.exceptions(stringstream::failbit | stringstream::badbit);
		try {
			if (cmd == "list_ng") {
				print_newsgroups(handler.send_list_ng());
			} else if (cmd == "list_art") {
				size_t ng_id;
				ss >> ng_id;

				try {
					print_articles(handler.send_list_art(ng_id));
				} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
					cerr << "No such newsgroup ID. Use 'list_ng' to see "
						"available newsgroups." << endl;
				}
			} else if (cmd == "read_art") {
				size_t ng_id;
				size_t art_id;
				ss >> ng_id;
				ss >> art_id;

				try {
					print_article(handler.send_get_article(ng_id, art_id));
				} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
					cerr << "No such newsgroup ID. Use 'list_ng' to see "
						"available newsgroups." << endl;
				} catch (client_msg_handler::article_does_not_exist_exception&) {
					cerr << "No such article ID. Use 'list_art " << ng_id << "' to "
						"see available articles in this newsgroup." << endl;
				}
			} else if (cmd == "create_ng") {
				string ng_name;
				ss >> ng_name;

				try {
					handler.send_create_ng(ng_name);
					cout << "Newsgroup created!" << endl;
				} catch (client_msg_handler::newsgroup_already_exists_exception&) {
					cerr << "A newsgroup of that name already exist, please "
						"choose another one." << endl;
				}
			} else if (cmd == "delete_ng") {
				int ng_id;
				ss >> ng_id;

				try {
					handler.send_delete_ng(ng_id);
					cout << "Newsgroup deleted!" << endl;
				} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
					cerr << "No such newsgroup ID. Use 'list_ng' to see "
						"available newsgroups." << endl;
				}
			} else if (cmd == "create_art") {
				size_t ng_id;
				string subject;
				string author;
				ss >> ng_id;

				cout << "Subject: ";
				getline(cin, subject);
				cout << "Author: ";
				getline(cin, author);

				try {
					char filename[L_tmpnam];
					if (!std::tmpnam(filename))
						cerr << "Failed to create temporary file." << endl;
					string command = string("$EDITOR ") + filename;
					int status = system(command.c_str());

					std::ifstream file(filename);
					if (file && status == 0) {
						string buf;
						// Find out length of content.
						file.seekg(0, file.end);
						std::ifstream::streampos filesize = file.tellg();
						buf.reserve(filesize);

						// Reset position and read content.
						file.seekg(0);
						char c;
						while ((c = file.get()) && !file.eof())
							buf += c;

						handler.send_create_art(ng_id, subject, author, buf);
						cout << endl << "Article created." << endl;
					}

					std::remove(filename);
				} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
					cerr << "No such newsgroup ID. Use 'list_ng' to see "
						"available newsgroups." << endl;
				}
			} else if (cmd == "delete_art") {
				size_t ng_id;
				size_t art_id;
				ss >> ng_id;
				ss >> art_id;

				try {
					handler.send_delete_art(ng_id, art_id);
					cout << "Article deleted!" << endl;
				} catch (client_msg_handler::newsgroup_does_not_exist_exception&) {
					cerr << "No such newsgroup ID. Use 'list_ng' to see "
						"available newsgroups." << endl;
				} catch (client_msg_handler::article_does_not_exist_exception&) {
					cerr << "No such article ID. Use 'list_art " << ng_id << "' to "
						"see available articles in this newsgroup." << endl;
				}
			} else if (cmd == "help") {
				cout << "Existing commands: " << endl;
				cout << " list_ng:\tLists the newsgroups, takes no parameters."
					<< endl;
				cout << " create_ng:\tCreate a new newsgroups, takes the name "
					"of the new newsgroup as a parameter." << endl;
				cout << " delete_ng:\tDeletes a newsgroups, takes the ID of the "
					"newsgroup as a parameter." << endl;
				cout << " list_art:\tLists the articles in a newsgroups, takes "
					"the ID if the newsgroup to list articles in as a parameter."
					<< endl;
				cout << " create_art:\tCreates a new article, takes the ID of "
					"the newsgroup to create the article in and then "
					"interactively asks for the rest of the information."
					<< endl;
				cout << " delete_art:\tDeletes an article, takes the ID of the "
					"newsgroup the article is in and the ID of the article as "
					"its parameters (in that order)." << endl;
				cout << " read_art:\tReads an article, takes the ID of the "
					"newsgroup the article is in and the ID of the article as "
					"its parameters (in that order)." << endl;
			} else {
				cerr << "Unknown command, use 'help' to list available commands."
					<< endl;
			}
		} catch (stringstream::failure&) {
			cerr << "Wrong parameters for this command. Use 'help' to see "
				<< "available commands." << endl;
		}

		// Reset exception state.
		ss.exceptions(oldstate);

		cout << endl << PROMPT;
	}

	return 0;
}
