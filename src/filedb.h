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

class file_database : public database {
public:
	
	struct io_access_error {};
	
	file_database(std::string dbdir = "./database/") throw(io_access_error) : top_path(dbdir) {
		ng_file = ".ngname.txt";
		top = opendir(dbdir.c_str());
		if (top == nullptr) 
			throw (io_access_error());
	};
	
	~file_database() {closedir(top);}
	
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

private:
	DIR* top;
	std::string top_path;
	std::string ng_file; //file that holds the newsgroup name
};

#endif
