#include "memdb.h"

/*	virtual vector<NG> list_ng() const;
	virtual Vector<Art> list_art(const NG&) const;
	virtual Art get_art(const NG&, size_t id) const;
	virtual NG create_ng(const NG&);
	virtual void delete_ng(size_t id);
	virtual Art create_art(const Art&);
	virtual void delete_art(size_t id);'
*/


vector<NG> MemDatabase::list_ng() {
	vector<NG> v;
	for (MDB::iterator it = db.begin(); it != db.end(); ++it) {
		v.push_back(it->first);
	}
	return v;
}


