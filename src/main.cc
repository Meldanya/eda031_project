#include "memdb.h"
#include <iostream>
#include <iterator>
#include <algorithm>

int main() {
	mem_database db = mem_database();
	db.create_ng("ng1");
	db.create_ng("ng2");
	
	std::vector<ng> ngs = db.list_ng();
	std::copy(ngs.begin(), ngs.end(), std::ostream_iterator<ng>(std::cout));
}
