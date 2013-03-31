#include "filedb.h"

#include <fstream>

ng file_database::get_ng(size_t id) const throw(ng_access_error) {
	DIR* dir = opendir((top_path + std::to_string(id)).c_str());
	if (dir == nullptr) 
		throw(ng_access_error());
	struct dirent* entry;
	while ((entry = readdir(dir)) != nullptr) {
		if (entry->d_name == ng_file) {
			std::ifstream ifs(top_path + std::to_string(id) + "/" + ng_file);
			std::string ng_name;
			ifs >> ng_name;
			ifs.close();
			return ng(id, ng_name);
		}
	}
}

ng file_database::create_ng(std::string name) throw(ng_access_error) {
	bool found = false;
	for (size_t id = 0; id != ng_ids; ++id) {
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
	
	int i = mkdir((top_path + std::to_string(id)).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (i == 0) {
		std::ofstream ofs(top_path + std::to_string(id) + "/" + ng_file);
		ofs << name;
		ofs.close();
		return ng(id, name);
	
	}
	//would prefer io_access_error, but not possible since it doesn't
	//allow us to throw exceptions not in base class.
	throw (ng_access_error()); 
	
}

std::vector<ng> file_database::list_ng() {
	return std::vector<ng>();
}

void file_database::delete_ng(size_t ng_id) throw(ng_access_error) {

}

void file_database::delete_art(size_t ng_id, size_t id) 
	throw(ng_access_error, art_access_error) {

}

art file_database::create_art(size_t ng_id, const std::string& author,
	const std::string& title, const std::string& content)
	throw(ng_access_error) {
		return art();
}

art file_database::get_art(size_t ng_id, size_t id) const
	throw(ng_access_error, art_access_error) {
		return art();
}

std::vector<art> file_database::list_art(size_t ng_id) const throw(ng_access_error) {
	return std::vector<art>();
}

