#ifndef DATABASE_H
#define DATABASE_H

#include "ng.h"
#include "art.h"

#include <string>
#include <vector>

/**
 * The interface for the database.
 */
class database {
public:

	struct art_access_error {}; //thrown when tried to access non-existing article
	struct ng_access_error {}; //thrown when tried to access non-existing newsgroup

	/**
	 * Constructs a new database, resets the ID's to 0.
	 */
	database() : art_ids(0), ng_ids(0) {}

	/**
	 * List the articles for the provided news group ID.
	 *
	 * @param ng_id The ID of the news group to list articles in.
	 *
	 * @return A std::vector of articles.
	 * @throws ng_access_error If the news group ID does not exist.
	 */
	virtual std::vector<art> list_art(size_t ng_id) const
			throw(ng_access_error) = 0;

	/**
	 * Get the article for the provided news group and article ID.
	 *
	 * @param ng_id The ID of the news group of the article.
	 * @param ng_id The ID of the article.
	 *
	 * @return The article as an art object.
	 * @throws ng_access_error If the news group ID does not exist.
	 * @throws art_access_error If the article ID does not exist.
	 */
	virtual art get_art(size_t ng_id, size_t id) const
			throw(ng_access_error, art_access_error) = 0;

	/**
	 * Creates a new article in the provided news group with the provided
	 * author, title and content.
	 *
	 * @param ng_id The ID of the news group to create the article in.
	 * @param author The name of the author of the article.
	 * @param title The title of the article.
	 * @param content The content of the article.
	 *
	 * @return The new article as an art object.
	 * @throws ng_access_error If the news group ID does not exist.
	 */
	virtual art create_art(size_t ng_id, const std::string& author,
			const std::string& title, const std::string& content)
			throw(ng_access_error) = 0;

	/**
	 * Deletes an article in the provided news group with the provided article ID.
	 *
	 * @param ng_id The ID of the news group to delete the article in.
	 * @param id The id of the article.
	 *
	 * @throws ng_access_error If the news group ID does not exist.
	 * @throws art_access_error If the article ID does not exist.
	 */
	virtual void delete_art(size_t ng_id, size_t id)
			throw(ng_access_error, art_access_error) = 0;

	/**
	 * Lists the news groups.
	 *
	 * @return A std::vector of ng objects.
	 */
	virtual std::vector<ng> list_ng() const = 0;

	/**
	 * Get the news group with the provided ID.
	 *
	 * @param id The ID of the news group.
	 *
	 * @return An ng object representing the news group.
	 * @throws ng_access_error If the news group ID doesn't exist.
	 */
	virtual ng get_ng(size_t id) const throw(ng_access_error) = 0;

	/**
	 * Create a new news group with the provided name.
	 *
	 * @param name The name of the news group.
	 *
	 * @return The new newsgroup as an ng object.
	 * @throws ng_access_error If the news group name already exist.
	 */
	virtual ng create_ng(std::string name) throw(ng_access_error) = 0;

	/**
	 * Delete the news group with the provided ID.
	 *
	 * @throws ng_access_error If the news group ID doesn't exist.
	 */
	virtual void delete_ng(size_t ng_id) throw(ng_access_error) = 0;

	/**
	 * Returns the next article ID to use.
	 *
	 * @return The next article ID to use.
	 */
	size_t max_art_id () { return art_ids++; }

	/**
	 * Returns the next news group ID to use.
	 *
	 * @return The next news group ID to use.
	 */
	size_t max_ng_id () { return ng_ids++; }

protected:
	size_t art_ids;
	size_t ng_ids;
};

#endif
