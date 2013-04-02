#ifndef CLIENT_MSG_HANDLER_H
#define CLIENT_MSG_HANDLER_H

#include "msg_handler.h"

#include "ng.h"
#include "art.h"
#include <vector>
#include <utility>

// TODO: Impl. this class
class client_msg_handler : public msg_handler
{
	public:
		/**
		 * A custom exception thrown if the server responds with the wrong
		 * answer.
		 */
		struct wrong_answer_exception {};
		/**
		 * A custom exception thrown if the server responds with ANS_NAK and
		 * ERR_NG_DOES_NOT_EXIST.
		 */
		struct newsgroup_does_not_exist_exception {};
		/**
		 * A custom exception thrown if the server responds with ANS_NAK and
		 * ERR_ART_DOES_NOT_EXIST.
		 */
		struct article_does_not_exist_exception {};
		/**
		 * A custom exception thrown if the server responds with ANS_NAK and
		 * ERR_NG_ALREADY_EXISTS.
		 */
		struct newsgroup_already_exists_exception {};

		client_msg_handler(client_server::Connection *conn)
			: msg_handler(conn) {}

		/**
		 * Sends the COM_LIST_NG command to the server, waits for response and
		 * returns the reply as a vector of newsgroups.
		 *
		 * @throws wrong_answer_exception If the server sends back the wrong
		 * reply.
		 *
		 * @return vector of ng's.
		 */
		std::vector<ng> send_list_ng();

		/**
		 * Sends the COM_LIST_ART command to the server, waits for the response
		 * and returns the response as a vector of pair<size_t, string> where
		 * pair.first is the article ID and pair.second is the newsgroup name.
		 *
		 * @param newsgroup_id The id of the newsgroup to list articles in.
		 *
		 * @throws wrong_answer_exception If the server replies with the wrong
		 * reply.
		 * @throws newsgroup_does_not_exist_exception If the newsgroup did not
		 * exist.
		 *
		 * @return vector of pair with ID and name of the articles.
		 */
		std::vector<std::pair<size_t, std::string>> send_list_art(
				int newsgroup_id);

		/**
		 * Sends the COM_GET_ART command to the server, waits for the response
		 * and returns the article.
		 *
		 * @param ng_id The id of the newsgroup to fetch from.
		 * @param art_id The id of the article to fetch.
		 *
		 * @throws wrong_answer_exception If the server responds with the wrong
		 * answer.
		 * @throws newsgroup_does_not_exist_exception If the newsgroup did not
		 * exist.
		 * @throws article_does_not_exist_exception If the article did not
		 * exist.
		 *
		 * @return The article.
		 */
		art send_get_article(size_t ng_id, size_t art_id);

		/**
		 * Sends the COM_CREATE_NG command to the server and waits for the
		 * response.
		 *
		 * @param newsgroup_name The newsgroup name.
		 *
		 * @throws wrong_answer_exception If the server responds with the wrong
		 * answer.
		 * @throws newsgroup_already_exists_exception If the newsgroup name
		 * already exists.
		 */
		void send_create_ng(const std::string &newsgroup_name);
		void send_delete_ng();
		void send_create_art();
		void send_delete_art();
};

#endif
