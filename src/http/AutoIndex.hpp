#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>

class	AutoIndex
{
// deleted
	AutoIndex	&operator=(const AutoIndex& autoIndex);

public:
// constructors & destructor
	AutoIndex();
	~AutoIndex();
	AutoIndex(const AutoIndex& autoIndex);

// member functions
	std::string autoIndex(std::string path);
};

#endif
