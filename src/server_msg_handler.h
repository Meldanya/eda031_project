#ifndef SERVER_MSG_HANDLER_H
#define SERVER_MSG_HANDLER_H

#include "msg_handler.h"
#include "database.h"

class server_msg_handler : public msg_handler
{
	public:
	/**
	 * Handle a request arriving on connection.
	 */
		void handle();

		/**
		 * Create an instance capable of handlinge requests on conn using
		 * database db.
		 *
		 * @param conn The connection to handle requests on.
		 * @param db   The database used to create responses.
		 */
		server_msg_handler(client_server::Connection *conn, database &db)
			: msg_handler(conn), db(db) {}

	private:
		database &db;

		/**
		 * Handles a request for news group listing.
		 */
		void handle_list_ng();

		/**
		 * Handles a request to create a newsgroup.
		 */
		void handle_create_ng();

		/**
		 * Handles a request to delete a newsgroup.
		 */
		void handle_delete_ng();

		/**
		 * Handles a request for a listing of the articles in a newsgroup.
		 */
		void handle_list_art();

		/**
		 * Handles a request to create an article.
		 */
		void handle_create_art();

		/**
		 * Handles a request to delete an article.
		 */
		void handle_delete_art();

		/**
		 * Handles a request to get an article.
		 */
		void handle_get_article();
};

#endif
