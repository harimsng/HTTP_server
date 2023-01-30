#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include <string>

class	AutoIndex
{
// deleted
	AutoIndex	&operator=(const AutoIndex& autoIndex);
	AutoIndex(const AutoIndex& autoIndex);

public:
// constructors & destructor
	AutoIndex();
	~AutoIndex();

// member functions
	static std::string autoIndex(const std::string& path);
};

#endif
