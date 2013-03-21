#ifndef MEMDB_H
#define MEMDB_H

#include <vector>
#include <map>
#include <string>

#include "database.h"
#include "ng.h"
#include "art.h"



class MemDatabase : public Database{
public:
	typedef std::map<NG,std::vector<Art>> MDB;
	
	virtual std::vector<NG> list_ng() const;
	/*virtual Vector<Art> list_art(const NG&) const;
	virtual Art get_art(const NG&, size_t id) const;
	virtual NG create_ng(const NG&);
	virtual void delete_ng(size_t id);
	virtual Art create_art(const Art&);
	virtual void delete_art(size_t id);*/
private:
	MDB db; 
};

#endif
