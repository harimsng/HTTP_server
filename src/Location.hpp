#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

class	Location
{
	friend class	LocationParser;
	friend class	ServerParser;

// deleted

public:
// constructors & destructor
	Location();
	~Location();
	Location(const Location& location);
//	Location&	operator=(const Location& location);

// member functions

private:
	void	setToDefault();

// member variables
	std::string		m_index;
	bool			m_autoindex;
	std::string		m_expires;
	std::string		m_proxyPass;
	std::string		m_limitExcept;
	std::string		m_path;
	std::string		m_root;
	std::string		m_cgiPass;
	std::string		m_alias;
	std::string		m_clientMaxBodySize;

	friend std::ostream&	operator<<(std::ostream& os, const Location& location);
};

#endif
