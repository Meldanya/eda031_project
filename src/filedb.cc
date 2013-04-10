#include "filedb.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Workaround for lack of std::to_string on Clang on Mac OSX
template <typename T>
std::string to_string(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

file_database::file_database(std::string dbdir) throw(io_access_error) : database(), top_path(dbdir),
								ng_file(".ngname.txt"), id_file("ids.txt"), separator("&SEPARATOR;;") {

	//Opens dir to check if it's reachable, writable etc.
	DIR* top = opendir(dbdir.c_str());
	if (top == nullptr) {
		int status = mkdir(dbdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (status != 0) 
			throw (io_access_error());
		top = opendir(dbdir.c_str());
	}
	
	//Parses the id file (if it exists) to update the max id variables.
	std::ifstream ifs((top_path + id_file).c_str());
	if (ifs.fail()) {
		ng_ids = 1;
		art_ids = 1;	
		ifs.close();
	} else {
		int id;
		ifs >> id;
		ng_ids = id;
		ifs >> id;
		art_ids = id;
		ifs.close();
	}
	
	closedir(top);
}


size_t file_database::max_ng_id() { 
	++ng_ids;
	std::ofstream ofs((top_path + id_file).c_str());
	ofs << ng_ids << " " << art_ids << std::endl;
	ofs.close();
	return ng_ids - 1;
}

size_t file_database::max_art_id () {
	++art_ids;
	std::ofstream ofs((top_path + id_file).c_str());
	ofs << ng_ids << " " << art_ids << std::endl;
	ofs.close();
	return art_ids - 1;
}

ng file_database::get_ng(size_t id) const throw(ng_access_error) {
	DIR* dir = opendir((top_path + to_string(id)).c_str());
	if (dir == nullptr) 
		throw(ng_access_error());
	//Go through database news group directory and look for the news group name file
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_name == ng_file) {
			//Parse the news group name file
			std::ifstream ifs((top_path + to_string(id) + "/" + ng_file).c_str());
			std::stringstream ss;
			ss << ifs.rdbuf();
			std::string ng_name(ss.str());
			ifs.close();
			closedir(dir);
			return ng(id, ng_name);
		}
	}
	closedir(dir);
	throw(ng_access_error());
}

ng file_database::create_ng(std::string name) throw(ng_access_error) {
	std::vector<ng> ngs(list_ng());
	
	//Look for existing news group with same name
	auto it = find_if(ngs.begin(), ngs.end(), [&name] (const ng& ng) { return ng.name == name; });
	if (it != ngs.end())
		throw(ng_access_error()); //ng with same name exists
	
	//no newsgroup with the name exists, create newsgroup folder
	size_t id = max_ng_id();
	
	int i = mkdir((top_path + to_string(id)).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (i == 0) {
		std::ofstream ofs((top_path + to_string(id) + "/" + ng_file).c_str());
		ofs << name;
		ofs.close();
		return ng(id, name);
	
	}
	//would prefer io_access_error, but not possible since it doesn't
	//allow us to throw exceptions not in base class.
	throw (ng_access_error()); 
	
}

std::vector<ng> file_database::list_ng() const {
	std::vector<ng> ngs;
	
	DIR* dir = nullptr; 
	//This should never happen. Will only happen if the entire database is removed during execution.
	if ((dir = opendir((top_path).c_str())) == nullptr) 
		return ngs; 
	
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		std::string name(entry->d_name);
		//Skips the id_file and "." and ".." and parses ng_id from folder name
		if (name != id_file && name != "." && name != "..") {
			std::stringstream ss(name);
			size_t ng_id;
			ss >> ng_id;
			ngs.push_back(get_ng(ng_id));
		}
	}
	sort(ngs.begin(), ngs.end(), [] (const ng& a, const ng& b) {return a.id < b.id;}); 
	closedir(dir);
	return ngs;

}

void file_database::delete_ng(size_t ng_id) throw(ng_access_error) {
	//Search for and destroy all articles in the news group
	std::vector<art> arts(list_art(ng_id));
	for_each(arts.begin(), arts.end(), [ng_id, this] (const art& a) { delete_art(ng_id, a.id); });

	//All articles in ng should be deleted
	
	int res = remove((top_path + to_string(ng_id) + "/" + ng_file).c_str());
	if (res != 0) 
		throw (ng_access_error());
		
	//Folder should be completely empty
	res = rmdir((top_path + to_string(ng_id)).c_str());
	if (res != 0) 
		throw (ng_access_error());
	
}

void file_database::delete_art(size_t ng_id, size_t id) 
					throw(ng_access_error, art_access_error) {
	
	DIR* dir = nullptr;
	if ((dir = opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	closedir(dir);
	int res = remove((top_path + to_string(ng_id) + "/" + to_string(id)).c_str());
	if (res != 0)
		throw (art_access_error());
	
}

art file_database::create_art(size_t ng_id, const std::string& author,
					const std::string& title, const std::string& content)
					throw(ng_access_error) {
	//Checks that the nd_id refers to a correct directory
	DIR* dir = nullptr;
	if ((dir = opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	closedir(dir);
	//Create a new article object
	art a(max_art_id(), author, title, content);
	//Write it to a file
	std::ofstream ofs((top_path + to_string(ng_id) + "/" + to_string(a.id)).c_str());
	ofs << author << separator
		<< title << separator
		<< content << separator;
	ofs.flush();
	ofs.close();
	return a;
}

art file_database::get_art(size_t ng_id, size_t id) const
					throw(ng_access_error, art_access_error) {
	
	//Check if directory for news group exists
	DIR* dir = nullptr;				
	if ((dir = opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	closedir(dir);
	//Read and parse the article
	std::ifstream ifs((top_path + to_string(ng_id) + "/" + to_string(id)).c_str());
	
	//This should not happen under normal circumstances, only if the article file is corrupted
	if (ifs.fail())
		throw (art_access_error());
		
	return parse(ifs, id);
	
}

art file_database::parse(std::ifstream& ifs, size_t art_id) const 
					throw (art_access_error)  {
	
	//Get it as a string
	std::stringstream ss;
	ss << ifs.rdbuf();
	std::string s(ss.str());
	ifs.close();
	
	//Find the separator strings
	size_t sep_length = separator.size();
	size_t first, second, third;
	first = s.find(separator);
	second = s.find(separator, first + sep_length);
	third = s.find(separator, second + sep_length);

	//Should only happen if article file is corrupted
	if (first == std::string::npos || second == std::string::npos || third == std::string::npos)
		throw(art_access_error());
	
	std::string author(s.substr(0,first));
	std::string title(s.substr(first + sep_length, second - (first + sep_length)));
	std::string content(s.substr(second + sep_length, third - (second + sep_length)));
	
	return art(art_id, author, title, content);
}


std::vector<art> file_database::list_art(size_t ng_id) const throw(ng_access_error) {
	DIR* dir = nullptr; 
	if ((dir = opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	
	std::vector<art> arts;
	
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		std::string name(entry->d_name);

		if (name != ng_file && name != "." && name != "..") {
			size_t art_id;
			std::stringstream ss(name);
			ss >> art_id;
			arts.push_back(get_art(ng_id,art_id));
		}
	}
	
	closedir(dir);
	
	//readdir gives them in seemingly random order, need to sort them by id
	sort(arts.begin(), arts.end(), [] (const art& a, const art& b) {return a.id < b.id;}); 
	
	return arts;
	
}

