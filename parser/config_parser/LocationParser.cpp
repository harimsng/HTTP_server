#include "Location.hpp"
#include "LocationParser.hpp"

using namespace std;

map<string, LocationParser::t_setter>	LocationParser::s_locationSetterMap;

void	LocationParser::setLocationSetterMap()
{
	s_locationSetterMap["index"] = &LocationParser::setIndex;
	s_locationSetterMap["expires"] = &LocationParser::setExpires;
	s_locationSetterMap["proxy_pass"] = &LocationParser::setProxyPass;
	s_locationSetterMap["limit_except"] = &LocationParser::setLimitExcept;
	s_locationSetterMap["path"] = &LocationParser::setPath;
	s_locationSetterMap["root"] = &LocationParser::setRoot;
	s_locationSetterMap["cgi_pass"] = &LocationParser::setCgiPass;
	s_locationSetterMap["alias"] = &LocationParser::setAlias;
	s_locationSetterMap["client_max_body_size"] = &LocationParser::setClientMaxBodySize;
}

// constructors & destructor
LocationParser::LocationParser(FileTokenizer& tokenizer)
:	m_tokenizer(tokenizer)
{
}

LocationParser::~LocationParser()
{
}

// member functions
void
LocationParser::parse(Location& location)
{
	while (s_locationSetterMap.count(m_tokenizer.peek()) == 1)
	{
		(this->*s_locationSetterMap[m_tokenizer.get()])(location);
	}
}

// setter
void
LocationParser::setIndex(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setExpires(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setProxyPass(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setLimitExcept(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setPath(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setRoot(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setCgiPass(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setAlias(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setClientMaxBodySize(Location& location)
{
	location.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}
