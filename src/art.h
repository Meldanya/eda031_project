#ifndef ART_H
#define ART_H

#include <string>
#include <iostream>


struct art {
	friend std::ostream& operator<<(std::ostream& os, const art& art);
	size_t id;
	std::string author;
	std::string title;
	std::string content;

	art(size_t id, std::string author, std::string title, std::string content) :
		id(id), author(author), title(title), content(content) {}

	art() : id(0), author(""), title(""), content("") {}

};

struct id_comparator {
	size_t id;
	id_comparator (size_t id) : id(id) {}

	bool operator()(const art& a) {
		return (a.id == id);
	}
};



#endif
