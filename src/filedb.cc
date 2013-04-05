#include "filedb.h"

#include <fstream>
#include <sstream>
#include <iostream>

// Stupid workaround 
template <typename T>
std::string to_string(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

file_database::file_database(std::string dbdir) throw(io_access_error) : database(), top_path(dbdir),
								ng_file(".ngname.txt"), id_file("ids.txt"), separator("&SEPARATOR;;") {

	DIR* top = opendir(dbdir.c_str());
	if (top == nullptr) {
		int status = mkdir(dbdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if (status != 0) 
			throw (io_access_error());
		top = opendir(dbdir.c_str());
	}
	
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
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_name == ng_file) {
			std::ifstream ifs((top_path + to_string(id) + "/" + ng_file).c_str());
			std::stringstream ss;
			ss << ifs.rdbuf();
			std::string ng_name(ss.str());
			ifs.close();
			return ng(id, ng_name);
		}
	}
	throw(ng_access_error());
}

ng file_database::create_ng(std::string name) throw(ng_access_error) {
	bool found = false;
	for (size_t id = 1; id != ng_ids; ++id) {
		if (!found) {
			try { 
				ng ng = get_ng(id);
				if (ng.name == name) {
					found = true;
				}
			} catch (ng_access_error) {
				//hole in newsgroup ids, just ignore and move on
			}
		}
	}
	if (found) 
		throw(ng_access_error());
		
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

	for (size_t id = 1; id != ng_ids; ++id) {
		try {

			ng ng = get_ng(id);
			ngs.push_back(ng);
		} catch (ng_access_error) {
			//do nothing

		}
	}
	
	return ngs;

}

void file_database::delete_ng(size_t ng_id) throw(ng_access_error) {
	for (size_t id = 1; id != art_ids; ++id) {
		try {
			delete_art(ng_id, id);
		} catch(art_access_error) {
			//do nothing, no such article and that's fine
		}
	}
	//all articles in ng are deleted
	
	int res = remove((top_path + to_string(ng_id) + "/" + ng_file).c_str());
	if (res != 0) 
		throw (ng_access_error());
	//folder should be completely empty
	res = rmdir((top_path + to_string(ng_id)).c_str());
	if (res != 0) 
		throw (ng_access_error());
	
}

void file_database::delete_art(size_t ng_id, size_t id) 
					throw(ng_access_error, art_access_error) {
	if ((opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	int res = remove((top_path + to_string(ng_id) + "/" + to_string(id)).c_str());
	if (res != 0)
		throw (art_access_error());
	
}

art file_database::create_art(size_t ng_id, const std::string& author,
					const std::string& title, const std::string& content)
					throw(ng_access_error) {
	if ((opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	art a(max_art_id(), author, title, content);
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
	if ((opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	std::ifstream ifs((top_path + to_string(ng_id) + "/" + to_string(id)).c_str());
	if (ifs.fail())
		throw (art_access_error());
	return parse(ifs, id);
	
}

art file_database::parse(std::ifstream& ifs, size_t art_id) const 
					throw (art_access_error)  {
	
	std::stringstream ss;
	ss << ifs.rdbuf();
	std::string s(ss.str());
	ifs.close();
	
	size_t sep_length = separator.size();
	size_t first, second, third;
	first = s.find(separator);
	second = s.find(separator, first + sep_length);
	third = s.find(separator, second + sep_length);

	if (first == std::string::npos || second == std::string::npos || third == std::string::npos)
		throw(art_access_error());
	
	std::string author(s.substr(0,first));
	std::string title(s.substr(first + sep_length, second - (first + sep_length)));
	std::string content(s.substr(second + sep_length, third - (second + sep_length)));
	
	return art(art_id, author, title, content);
}


std::vector<art> file_database::list_art(size_t ng_id) const throw(ng_access_error) {
	if ((opendir((top_path + to_string(ng_id)).c_str())) == nullptr) 
		throw (ng_access_error());
	
	std::vector<art> arts;
	for (size_t id = 1; id != art_ids; ++id) {
		try {
			art a(get_art(ng_id, id));
			arts.push_back(a);
		} catch (art_access_error) {
			//no such article, just move on
		}
	}
	return arts;
	
}

