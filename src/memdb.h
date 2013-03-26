#ifndef MEMDB_H
#define MEMDB_H

#include <vector>
#include <map>
#include <string>

#include "database.h"
#include "ng.h"
#include "art.h"

class mem_database : public database {
public:
	typedef std::map<ng,std::vector<art>> mdb;
	typedef std::map<size_t, ng> idmap;

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
	mdb db;
	idmap ids;
};

#endif
