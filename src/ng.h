#ifndef NG_H
#define NG_H

#include <string>



struct NG {
	size_t id;
	std::string name;
	
	NG (size_t id, std::string name) : id(id), name(name) {}
	
	bool operator<(const NG& other) {
		return id < other.id;
	}
};

#endif
