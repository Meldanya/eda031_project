#ifndef ART_H
#define ART_H

#include <string>



struct Art { 
	size_t id;
	std::string author;
	std::string title;
	std::string content;
	
	Art(size_t id, std::string author, std::string title, std::string content) : 
		id(id), author(author), title(title), content(content) {}
	
};

#endif
