#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

class	Location
{
	friend class	LocationParser;

// deleted
	Location	&operator=(Location const& location);

public:
// constructors & destructor
	Location();
	~Location();
	Location(Location const& location) {(void)location;};

// member functions

// member variables
	std::string		m_index;
	std::string		m_expires;
	std::string		m_proxyPass;
	std::string		m_limitExcept;
	std::string		m_path;
	std::string		m_root;
	std::string		m_cgiPass;
	std::string		m_alias;
	std::string		m_clientMaxBodySize;
};

#endif
