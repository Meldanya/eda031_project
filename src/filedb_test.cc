#include "filedb.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

int main() {
	file_database db;


	
	vector<ng> ngs = db.list_ng();
	cout << ngs.size() << endl;
	copy(ngs.begin(), ngs.end(), ostream_iterator<ng>(cout));
}
