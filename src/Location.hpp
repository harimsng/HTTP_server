#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>

class	Location
{
	friend class	LocationParser;
	friend class	ServerParser;
	friend class	FindLocation;
	friend class	AMethod;
	friend class	GetMethod;

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
public:
	std::string		m_index;

	bool			m_autoindex;

	std::string		m_expires;

	std::string		m_proxyPass;

	std::string		m_scgiPass;
	std::string		m_fastcgiPass;

	std::string		m_path;
	std::string		m_root;
	std::string		m_alias;
	std::string		m_limitExcept;
	std::string		m_clientMaxBodySize;
//	std::string		m_errorPageTable[599]; // index is status code


	friend std::ostream&	operator<<(std::ostream& os, const Location& location);
};

#endif
