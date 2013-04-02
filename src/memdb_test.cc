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
	try {
		art a = db.get_art(ngs[0].id, 1);
	} catch (database::art_access_error e) {
		cout << "Correctly caught art exception" << endl;
	}
	
	try {
		ng n = db.get_ng(2);
	} catch (database::ng_access_error) {
		cout << "Correctly caught ng exception" << endl;
	}
	
	db.delete_art(ngs[0].id, 1);
	db.delete_ng(2);
	db.create_art(ngs[0].id, "Walde", "Snelare hest", "DERPIER\n");
	
	ngs = db.list_ng();
	copy(ngs.begin(), ngs.end(), ostream_iterator<ng>(cout));
	
	arts = db.list_art(ngs[0].id);
	copy(arts.begin(), arts.end(), ostream_iterator<art>(cout));
	
	db.create_ng("ng3, should have id 2");
	
	ngs = db.list_ng();
	copy(ngs.begin(), ngs.end(), ostream_iterator<ng>(cout));

	
}
