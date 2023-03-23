#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Types.hpp"

#include <string>
#include <vector>
#include <map>

class	Location
{

public:
// constructors & destructor
	Location();
	~Location();

// member functions

private:
	void	setToDefault();

// member variables
public:
	int							m_autoindex;
	t_uint16					m_limitExcept;
	t_int32						m_clientMaxBodySize;
	std::string					m_path;
	std::string					m_root;
	std::string					m_alias;
	std::string					m_return;
	std::vector<std::string>	m_index;
	std::map<int, std::string>	m_errorPageTable;

	friend std::ostream&	operator<<(std::ostream& os, const Location& location);
};

#endif
