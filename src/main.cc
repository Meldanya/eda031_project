#include "memdb.h"
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
	mem_database db = mem_database();
	db.create_ng("ng1");
	db.create_ng("ng2");
	
	vector<ng> ngs = db.list_ng();
	copy(ngs.begin(), ngs.end(), ostream_iterator<ng>(cout));
	
	db.create_art(ngs[0].id, "T-Bone", "Snel hest", "Jak er snel hest\nDERP");
	vector<art> arts = db.list_art(ngs[0].id);
	copy(arts.begin(), arts.end(), ostream_iterator<art>(cout));
	
	art a = db.get_art(ngs[0].id, 0);
}
