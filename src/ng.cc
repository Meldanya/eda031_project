#include "ng.h"

std::ostream& operator<<(std::ostream& os, const ng& ng) {
	os << ng.id << " : " << ng.name << std::endl;
	return os;
}

