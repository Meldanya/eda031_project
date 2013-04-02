#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include "connection.h"
#include "database.h"

#include <vector>


class msg_handler
{
	public:
		typedef struct malformed_req_exception {} malformed_req_exception;

		/**
		 * Creates an instance for handling messages on a specified connection.
		 *
		 * @param conn The connection th handle messages on.
		 */
		msg_handler(client_server::Connection *conn) : conn(conn) {}

	protected:
		client_server::Connection *conn;

		/**
		 * Reads the end byte of the protocol from the channel.
		 *
		 * TODO: Move to server_msg_handler.
		 */
		void read_end(); //throw(client_server::ConnectionClosedException);

		/**
		 * Read a four byte int from the connection.
		 *
		 * @return The read integer.
		 */
		int read_int();

		/**
		 * Reads a string in protocol format from the connection.
		 */
		std::string read_parameter_string();

		/**
		 * Reads an integer in protocol format from the connection.
		 *
		 * @return The read integer.
		 */
		int read_parameter_int();

		/**
		 * Writes a four byte integer to the connection.
		 * 
		 * @param value The integer to write.
		 */
		void write_int(int value);

		/**
		 * Writes a string in protocol format to the connection.
		 *
		 * @param s The string to write.
		 */
		void write_string(const std::string &s);

		/**
		 * Writes an integer in protocol format to the connection.
		 *
		 * @param value The integer to write.
		 */
		void write_parameter_int(int value);
};

#endif

