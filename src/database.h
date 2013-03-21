#ifndef DATABASE_H
#define DATABASE_H

#include "ng.h"
#include "art.h"

#include <string>
#include <vector>



class Database {
public:
	virtual std::vector<NG> list_ng() const = 0;
	virtual std::vector<Art> list_art(const NG&) const = 0;
	virtual Art get_art(const NG&, size_t id) const = 0;
	virtual NG create_ng(const NG&) = 0;
	virtual void delete_ng(size_t id) = 0;
	virtual Art create_art(const Art&) = 0;
	virtual void delete_art(size_t id) = 0;
	
};


#endif
