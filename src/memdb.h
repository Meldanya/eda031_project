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
	mem_database() {
		art_ids = 0;
		ng_ids = 0;
	}
	
	typedef std::map<ng,std::vector<art>> mdb;
	typedef std::map<size_t, ng> idmap;
	
	virtual articles list_art(size_t ng_id) const throw(access_error);
	virtual art get_art(size_t ng_id, size_t id) const throw(access_error);
	virtual art create_art(size_t ng_id, const art&) throw(access_error);
	virtual void delete_art(size_t ng_id, size_t id) throw(access_error);
	
	virtual news_groups list_ng() const;
	virtual ng get_ng(size_t id) const throw(access_error);
	virtual ng create_ng(const ng&);
	virtual void delete_ng(size_t ng_id) throw(access_error);
	
private:
	mdb db; 
	idmap ids;
};

#endif
