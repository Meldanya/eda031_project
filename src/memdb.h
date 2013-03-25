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
	
	virtual news_groups list_ng() const;
	virtual ng get_ng(size_t id) const throw(access_error);
	virtual articles list_art(const ng&) const throw(access_error);
	virtual art get_art(const ng&, size_t id) const throw(access_error);
	virtual ng create_ng(const ng&);
	virtual void delete_ng(const ng&) throw(access_error);
	virtual art create_art(const ng&, const art&) throw(access_error);
	virtual void delete_art(const ng&, size_t id) throw(access_error);
private:
	mdb db; 
};

#endif
