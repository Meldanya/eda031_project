#include "filedb.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

int main() {
	file_database db;

	//db.delete_art(2,4);
	//db.create_art(2,"Walde", "Hest derpman", "herpaderp");
	
	//db.create_art(1,"TB", "Hest", "Snela hestar");

	//db.create_ng("hej hej");
	//db.create_ng("hestar");

	//db.delete_ng(1);

	vector<ng> ngs = db.list_ng();
	copy(ngs.begin(), ngs.end(), ostream_iterator<ng>(cout));
	
	vector<art> arts = db.list_art(2);
	copy(arts.begin(), arts.end(), ostream_iterator<art>(cout));
	
}
