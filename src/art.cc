#include "art.h"

std::ostream& operator<<(std::ostream& os, const art& art) {
	os << "id: " << art.id << "\nauthor = " << art.author << 
		", title = " << art.title << "\n" << art.content << std::endl;
	return os;
}
