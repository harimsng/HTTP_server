#include <sstream>
#include <algorithm>

#include "exception/ConfigParserException.hpp"
#include "Server.hpp"
#include "parser/config_parser/ConfigParser.hpp"
#include "parser/config_parser/LocationParser.hpp"
#include "parser/config_parser/ServerParser.hpp"

using namespace std;

map<string, ServerParser::t_setter>	ServerParser::s_serverSetterMap;

void	ServerParser::setServerSetterMap()
{
	s_serverSetterMap["index"] = &ServerParser::setIndex;
	s_serverSetterMap["server_names"] = &ServerParser::setServerNames;
	s_serverSetterMap["error_code"] = &ServerParser::setErrorCode;
	s_serverSetterMap["error_pages"] = &ServerParser::setErrorPages;
	s_serverSetterMap["root"] = &ServerParser::setRoot;
	s_serverSetterMap["listen"] = &ServerParser::setListenAddress;
	s_serverSetterMap["client_max_body_size"] = &ServerParser::setClientMaxBodySize;
	s_serverSetterMap["uri_buffer_size"] = &ServerParser::setUriBufferSize;
	s_serverSetterMap["location"] = &ServerParser::parseLocation;
}

// constructors & destructor
ServerParser::ServerParser(FileTokenizer& m_tokenizer)
:	m_tokenizer(m_tokenizer)
{
}

ServerParser::~ServerParser()
{
}

#include <iostream>

// member functions
void
ServerParser::parse(Server& server)
{
	while (s_serverSetterMap.count(m_tokenizer.peek()) == 1)
	{
		(this->*s_serverSetterMap[m_tokenizer.get()])(server);
	}
}

void
ServerParser::parseLocation(Server& server)
{
	string	locationPath = m_tokenizer.get();

	m_tokenizer.eat("{");

	LocationParser	locationParser(m_tokenizer);

	server.m_locationList.push_back(Location());
	server.m_locationList.back().m_path = locationPath;
	locationParser.parse(server.m_locationList.back());
	m_tokenizer.eat("}");
}

// setters
void
ServerParser::setIndex(Server& server)
{
	server.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setServerNames(Server& server)
{
	server.m_serverNames = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setListenAddress(Server& server)
{
	string				listenField = m_tokenizer.get();
	stringstream		ss;
	string::size_type	colonPos = listenField.find(":");
	uint32_t			addr = 0;
	uint16_t			port = 0;

	if (colonPos != string::npos || count(listenField.begin(), listenField.end(), '.') > 0)
	{
		uint16_t	addrPart = 0;
		char		period;

		ss.str(listenField.substr(0, colonPos));
		for (int i = 0; i < 3; ++i)
		{
			ss >> addrPart >> period;
			if (period != '.' || ss.fail() || addrPart > 255)
				throw ConfigParser::ConfigParserException("invalid listen address");
			addr = (addr << 8) | addrPart;
		}
		ss >> addrPart;
		addr = (addr << 8) | addrPart;
		if (ss.eof() == false || ss.fail() == true)
			throw ConfigParser::ConfigParserException("invalid listen address");
	}

	if (!(colonPos == string::npos && ss.eof() == true))
	{

		if (listenField.rfind(":") != colonPos)
			throw ConfigParser::ConfigParserException("invalid listen address");
		ss.clear();
		ss.str(colonPos != string::npos ? listenField.substr(colonPos + 1, string::npos) : listenField);
		ss >> port;
		if (ss.eof() == false || ss.fail() == true)
			throw ConfigParser::ConfigParserException("invalid listen address");
	}
	server.m_listen = GET_SOCKADDR_IN(addr, port);
	m_tokenizer.eat(";");
}

void
ServerParser::setRoot(Server& server)
{
	server.m_root = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorPages(Server& server)
{
	server.m_errorPages = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorCode(Server& server)
{
	server.m_errorCode = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setClientMaxBodySize(Server& server)
{
	server.m_clientMaxBodySize = ConfigParser::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}

void
ServerParser::setUriBufferSize(Server& server)
{
	server.m_uriBufferSize = ConfigParser::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}
