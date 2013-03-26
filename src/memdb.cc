#include "memdb.h"
#include <algorithm>


database::news_groups mem_database::list_ng() const {
	database::news_groups v;
	for (mdb::const_iterator it = db.cbegin(); it != db.cend(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

ng mem_database::get_ng(size_t id) const throw(database::ng_access_error) {
	auto it = ids.find(id);
	if (it != ids.cend()) {
		return it->second;
	}
	throw(database::ng_access_error());
}

database::articles mem_database::list_art(size_t id) const throw(database::ng_access_error) {
	ng ng = get_ng(id);
	
	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		return it->second;
	}
	throw(database::ng_access_error());
}

art mem_database::get_art(size_t ng_id, size_t art_id) const throw(database::ng_access_error, database::art_access_error) {
	ng ng = get_ng(ng_id);

	mdb::const_iterator it = db.find(ng);
	if (it != db.cend()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.cbegin(), arts.cend(), id_comparator(art_id));
		if (found != arts.cend()) {
			return *found;
		} 
		throw(database::art_access_error());
		
	} else {
		throw(database::ng_access_error());
	}

	
}

ng mem_database::create_ng(std::string name) throw(database::ng_access_error) {
	//find another ng with same name if it exists
	for (auto it = ids.begin(); it != ids.end(); ++it) {
		if (it->second.name == name) {
			throw(database::ng_access_error());
		}
	}
	ng ng(max_ng_id(),name);
	ids[ng.id] = ng;
	db[ng] = database::articles();
	return ng;
}

void mem_database::delete_ng(size_t ng_id) throw(database::ng_access_error) {
	ng ng = get_ng(ng_id);

	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		db.erase(it);
		return;
	}
	throw(database::ng_access_error());
	
}

art mem_database::create_art(size_t ng_id, const std::string& author, const std::string& title, 
							 const std::string& content) throw(database::ng_access_error) {
	art art(max_art_id(), author, title, content);	
	ng ng = get_ng(ng_id);
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		it->second.push_back(art);
		return art;
	}
	throw (database::ng_access_error());
}

void mem_database::delete_art(size_t ng_id, size_t art_id) throw(database::ng_access_error, database::art_access_error) {
	ng ng = get_ng(ng_id);
	
	mdb::iterator it = db.find(ng);
	if (it != db.end()) {
		database::articles arts = it->second;
		
		auto found = std::find_if(arts.begin(), arts.end(), id_comparator(art_id));
		if (found != arts.end()) {
			arts.erase(found);
			return;
		}
		throw(database::art_access_error());	
	}
	throw(database::ng_access_error());
}
