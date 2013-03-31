#include "memdb.h"
#include <algorithm>

std::vector<ng> mem_database::list_ng() const {
	std::vector<ng> v;
	for (mdb::const_iterator it = db.begin(); it != db.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}

ng mem_database::get_ng(size_t id) const throw(database::ng_access_error) {
	auto it = ids.find(id);
	if (it != ids.end()) {
		return it->second;
	}
	throw(database::ng_access_error());
}

std::vector<art> mem_database::list_art(size_t id) const throw(database::ng_access_error) {
	ng ng = get_ng(id);

	mdb::const_iterator it = db.find(ng);
	if (it != db.end()) {
		return it->second;
	}
	throw(database::ng_access_error());
}

art mem_database::get_art(size_t ng_id, size_t art_id) const throw(database::ng_access_error, database::art_access_error) {
	ng ng = get_ng(ng_id);

	mdb::const_iterator it = db.find(ng);
	if (it != db.end()) {
		std::vector<art> arts = it->second;

		auto found = std::find_if(arts.begin(), arts.end(), id_comparator(art_id));
		if (found != arts.end()) {
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
	db[ng] = std::vector<art>();
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
		std::vector<art>& arts = it->second;

		auto found = std::find_if(arts.begin(), arts.end(), id_comparator(art_id));
		if (found != arts.end()) {
			arts.erase(found);
			return;
		}
		throw(database::art_access_error());
	}
	throw(database::ng_access_error());
}
