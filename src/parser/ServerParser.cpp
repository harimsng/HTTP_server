#include <sstream>
#include <algorithm>

#include "exception/ConfigParserException.hpp"
#include "event/Server.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/LocationParser.hpp"
#include "parser/ServerParser.hpp"

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
ServerParser::parse(VirtualServer& server)
{
	while (s_serverSetterMap.count(m_tokenizer.peek()) == 1)
	{
		(this->*s_serverSetterMap[m_tokenizer.get()])(server);
	}
}

void
ServerParser::parseLocation(VirtualServer& server)
{
	string	locationPath = m_tokenizer.get();

	m_tokenizer.eat("{");

	LocationParser	locationParser(m_tokenizer);

	Location	location;

	location.m_path = locationPath;
	locationParser.parse(location);
	server.m_locationList[locationPath] = location;

	// server.m_locationTable[];
	// server.m_locationList.push_back(Location());
	// server.m_locationList.back().m_path = locationPath;
	// locationParser.parse(server.m_locationList.back());
	m_tokenizer.eat("}");
}

// setters
void
ServerParser::setIndex(VirtualServer& server)
{
	server.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setServerNames(VirtualServer& server)
{
	// INFO: it would be better if token type is added for the token data.
	server.m_serverNames.clear();
	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		server.m_serverNames.push_back(m_tokenizer.get());
	}
	m_tokenizer.eat(";");
}

void
ServerParser::setListenAddress(VirtualServer& server)
{
	string				listenField = m_tokenizer.get();
	stringstream		ss;
	string::size_type	colonPos = listenField.find(":");
	uint64_t			addr = 0;
	uint16_t			port = 0;

	// TODO: we can simplify this function by using getaddrinfo(), not necessary though.
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
	server.m_addrKey = (addr << 16) + port;
	LOG(Logger::DEBUG, "listen field : %s\n addr : %u \naddr << 16 : %llu", listenField.c_str(), addr, (addr << 16));
	m_tokenizer.eat(";");
}

void
ServerParser::setRoot(VirtualServer& server)
{
	server.m_root = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorPages(VirtualServer& server)
{
	server.m_errorPages = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorCode(VirtualServer& server)
{
	server.m_errorCode = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setClientMaxBodySize(VirtualServer& server)
{
	server.m_clientMaxBodySize = Util::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}

void
ServerParser::setUriBufferSize(VirtualServer& server)
{
	server.m_uriBufferSize = Util::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}
