#ifndef FILEDB_H
#define FILEDB_H

#include <vector>
#include <string>

#include <dirent.h> //*dir
#include <sys/stat.h> //mkdir
#include <cstdio> //remove
#include <sys/types.h> //mkdir types

#include "database.h"
#include "ng.h"
#include "art.h"

/**
 * Database that stores all data on disk and thus the data is kept even after the object is destroyed.
 * All changes are instantly reflected on disk.
 */

class file_database : public database {
public:
	struct io_access_error {}; //Used in file-specific errors, i.e. constructor fail.
	
	file_database(std::string dbdir = "./database/") throw(io_access_error);
	
	~file_database() { }
	
	virtual std::vector<art> list_art(size_t ng_id) const throw(ng_access_error);
	
	virtual art get_art(size_t ng_id, size_t id) const
			throw(ng_access_error, art_access_error);
	
	virtual art create_art(size_t ng_id, const std::string& author,
			const std::string& title, const std::string& content)
			throw(ng_access_error);
	virtual void delete_art(size_t ng_id, size_t id)
			throw(ng_access_error, art_access_error);

	virtual std::vector<ng> list_ng() const;
	virtual ng get_ng(size_t id) const throw(ng_access_error);
	virtual ng create_ng(std::string name) throw(ng_access_error); 
	virtual void delete_ng(size_t ng_id) throw(ng_access_error);

	virtual size_t max_ng_id ();
	virtual size_t max_art_id (); 

private:

	const std::string top_path;
	const std::string ng_file; //file that holds the newsgroup name
	const std::string id_file; //file that holds the max ids
	
	const std::string separator; //inside each article file this string separates author, title and content.
	
	/**
	 * Parses an ifstream with the file contents of an article into an article object.
	 *
	 * @param ifs The stream containing the content of the article file.
	 * @param art_id The id of the article.
	 *
	 * @return The article as an art object.
	 * @throws art_access_error If the article ID does not exist or on read error.
	 */
	art parse(std::ifstream& ifs, size_t art_id) const throw(art_access_error);
};

#endif
