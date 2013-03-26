#ifndef DATABASE_H
#define DATABASE_H

#include "ng.h"
#include "art.h"

#include <string>
#include <vector>


class database {
public:
	
	struct access_error {}; //thrown when tried to access non-existing articles or newgroups
	
	typedef std::vector<ng> news_groups;
	typedef std::vector<art> articles;
	
	virtual articles list_art(size_t ng_id) const throw(access_error) = 0 ;
	virtual art get_art(size_t ng_id, size_t id) const throw(access_error) = 0;
	virtual art create_art(size_t ng_id, const art&) throw(access_error) = 0;
	virtual void delete_art(size_t ng_id, size_t id) throw(access_error) = 0;
	
	virtual news_groups list_ng() const = 0;
	virtual ng get_ng(size_t id) const throw(access_error) = 0;	
	virtual ng create_ng(const ng&) = 0;
	virtual void delete_ng(size_t ng_id) throw(access_error) = 0;
	
	size_t max_art_id () { return art_ids++; }
	size_t max_ng_id () { return ng_ids++; }

protected:
	size_t art_ids;
	size_t ng_ids;
};


#endif
