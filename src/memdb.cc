#include "memdb.h"
#include <algorithm>



database::news_groups mem_database::list_ng() const {
	database::news_groups v;
	for (mdb::const_iterator it = db.cbegin(); it != db.cend(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

ng mem_database::get_ng(size_t id) const throw(database::access_error) {
	for (auto it = db.cbegin(); it != db.cend(); ++it) {
		auto tmp = it->first;
		if (tmp.id == id) {
			return tmp;
		}
	}
	throw(database::access_error());
}

database::articles mem_database::list_art(const ng& ng) const throw(database::access_error) {
	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		return it->second;
	}
	throw(database::access_error());
}

art mem_database::get_art(const ng& ng, size_t id) const throw(database::access_error) {
	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.cbegin(), arts.cend(), id_comparator(id));
		if (found != arts.cend()) {
			return *found;
		} 
		
	} 
	throw(database::access_error());
	
}

ng mem_database::create_ng(const ng& ng) {
	db[ng] = database::articles();
	return ng;
}

void mem_database::delete_ng(const ng& ng) throw(database::access_error) {
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		db.erase(it);
		return;
	}
	throw(database::access_error());
	
}

art mem_database::create_art(const ng& ng, const art& art) throw(database::access_error) {
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		it->second.push_back(art);
		return art;
	}
	throw (database::access_error());
}

void mem_database::delete_art(const ng& ng, size_t id) throw(database::access_error) {
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.begin(), arts.end(), id_comparator(id));
		if (found != arts.end()) {
			arts.erase(found);
			return;
		}	
	}
	throw(database::access_error());
}
