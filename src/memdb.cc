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
	auto it = ids.find(id);
	if (it != ids.cend()) {
		return it->second;
	}
	throw(database::access_error());
}

database::articles mem_database::list_art(size_t id) const throw(database::access_error) {
	ng ng = get_ng(id);
	
	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		return it->second;
	}
	throw(database::access_error());
}

art mem_database::get_art(size_t ng_id, size_t art_id) const throw(database::access_error) {
	ng ng = get_ng(ng_id);

	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.cbegin(), arts.cend(), id_comparator(art_id));
		if (found != arts.cend()) {
			return *found;
		} 
		
	} 
	throw(database::access_error());
	
}

ng mem_database::create_ng(const ng& ng) {
	ids[ng.id] = ng;
	db[ng] = database::articles();
	return ng;
}

void mem_database::delete_ng(size_t ng_id) throw(database::access_error) {
	ng ng = get_ng(ng_id);

	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		db.erase(it);
		return;
	}
	throw(database::access_error());
	
}

art mem_database::create_art(size_t ng_id, const art& art) throw(database::access_error) {
	ng ng = get_ng(ng_id);
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		it->second.push_back(art);
		return art;
	}
	throw (database::access_error());
}

void mem_database::delete_art(size_t ng_id, size_t art_id) throw(database::access_error) {
	ng ng = get_ng(ng_id);
	
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.begin(), arts.end(), id_comparator(art_id));
		if (found != arts.end()) {
			arts.erase(found);
			return;
		}	
	}
	throw(database::access_error());
}
