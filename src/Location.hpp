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

//	Location(const Location& location) = default;
//	Location&	operator=(const Location& location) = default;

// member functions

private:
	void	setToDefault();

// member variables
public:
	int									m_autoindex;
	t_uint16							m_limitExcept;
	t_int32								m_clientMaxBodySize;
	std::string							m_path;
	std::string							m_root;
	std::string							m_alias;
	std::vector<std::string>			m_index;
	std::map<int, std::string>			m_errorPageTable;

	//std::vector<std::string>	m_cgiExt;

	friend std::ostream&	operator<<(std::ostream& os, const Location& location);
};

#endif
