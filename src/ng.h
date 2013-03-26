#ifndef NG_H
#define NG_H

#include <string>



struct ng {
	size_t id;
	std::string name;
	
	ng (size_t id, std::string name) : id(id), name(name) {}
	
	ng() {id = 0; name = "";}
	
	bool operator<(const ng& other) const {
		return id < other.id;
	}
};

#endif
