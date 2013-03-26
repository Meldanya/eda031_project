#ifndef ART_H
#define ART_H

#include <string>
#include <iostream>


struct art { 
	friend std::ostream& operator<<(std::ostream& os, const art& art);
	size_t id;
	typedef std::string string;
	string author;
	string title;
	string content;
	
	art(size_t id, string author, string title, string content) : 
		id(id), author(author), title(title), content(content) {}
	
	
};



struct id_comparator {
	size_t id;
	id_comparator (size_t id) : id(id) {}
	
	bool operator()(const art& a) {
		return (a.id == id);
	}
};
#endif
