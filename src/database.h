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
	virtual news_groups list_ng() const = 0;
	virtual ng get_ng(size_t id) const throw(access_error) = 0;
	virtual articles list_art(const ng&) const throw(access_error) = 0 ;
	virtual art get_art(const ng&, size_t id) const throw(access_error) = 0;
	virtual ng create_ng(const ng&) = 0;
	virtual void delete_ng(const ng&) throw(access_error) = 0;
	virtual art create_art(const ng&, const art&) throw(access_error) = 0;
	virtual void delete_art(const ng&, size_t id) throw(access_error) = 0;
	
	size_t max_art_id () { return art_ids++; }
	size_t max_ng_id () { return ng_ids++; }
	
	
private:
	size_t art_ids = 0;
	size_t ng_ids = 0;
};


#endif
