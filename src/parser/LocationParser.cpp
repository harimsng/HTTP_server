#include "Logger.hpp"
#include "Location.hpp"
#include "http/RequestHandler.hpp"
#include "parser/LocationParser.hpp"

using namespace std;

map<string, LocationParser::t_setter>	LocationParser::s_locationSetterMap;

void
LocationParser::setLocationSetterMap()
{
	s_locationSetterMap["index"] = &LocationParser::setIndex;
	s_locationSetterMap["limit_except"] = &LocationParser::setLimitExcept;
	s_locationSetterMap["path"] = &LocationParser::setPath;
	s_locationSetterMap["root"] = &LocationParser::setRoot;
	s_locationSetterMap["cgi_pass"] = &LocationParser::setCgiPass;
	s_locationSetterMap["alias"] = &LocationParser::setAlias;
	s_locationSetterMap["client_max_body_size"] = &LocationParser::setClientMaxBodySize;
	s_locationSetterMap["error_page"] = &LocationParser::setErrorPage;
	s_locationSetterMap["autoindex"] = &LocationParser::setAutoIndex;

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
	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		location.m_index.push_back( m_tokenizer.get());
	}
	m_tokenizer.eat(";");
}

void
LocationParser::setLimitExcept(Location& location)
{
	string	method;

	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		method = m_tokenizer.get();
		location.m_limitExcept |= RequestHandler::s_methodConvertTable[method];
		LOG(DEBUG, "method = %s, m_limitExcept = %x", method.c_str(), location.m_limitExcept);
	}
	m_tokenizer.eat(";");
}

void
LocationParser::setPath(Location& location)
{
	location.m_path = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setRoot(Location& location)
{
	location.m_root = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setCgiPass(Location& location)
{
	location.m_cgiPass = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setAlias(Location& location)
{
	location.m_alias = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
LocationParser::setClientMaxBodySize(Location& location)
{
	location.m_clientMaxBodySize = Util::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}

void
LocationParser::setErrorPage(Location& location)
{
	vector<string> token;

	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		token.push_back(m_tokenizer.get());
	}
	for (size_t i = 0; i < token.size() - 1; i++)
	{
		location.m_errorPageTable[Util::toInt(token[i])] = token.back();
	}
	m_tokenizer.eat(";");

}
