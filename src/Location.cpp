#include <ostream>

#include "Location.hpp"

using namespace std;

// constructors & destructor
Location::Location()
{
}

Location::~Location()
{
}

Location::Location(const Location& location)
{
	*this = location;
}

/*
Location&	Location::operator=(const Location& location)
{
	m_path = location.m_path;
	m_alias = location.m_alias;
	m_index = location.m_index;
	m_autoindex = location.m_autoindex;
	m_root = location.m_root;
	return *this;
}
*/

void
Location::setToDefault()
{
	m_index = "index.html";
	m_autoindex = false;
	// m_expires;
	// m_proxyPass;
	// m_limitExcept;
	// m_path;
	// m_root;
	// m_cgiPass;
	// m_alias;
	// m_clientMaxBodySize;
}

ostream&
operator<<(std::ostream& os, const Location& location)
{
	os << "\tlocation\t" << location.m_path << "\n\t{\n";
	os << "\t\talias " << location.m_alias << "\n";
	os << "\t\tindex " << location.m_index << "\n";
	os << "\t\troot " << location.m_root << "\n";
	os << "\t}\n";
	return os;
}
