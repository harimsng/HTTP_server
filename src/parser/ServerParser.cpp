#include <sstream>
#include <algorithm>

#include "Logger.hpp"
#include "util/Util.hpp"
#include "exception/ConfigParserException.hpp"
#include "event/Server.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/LocationParser.hpp"
#include "parser/ServerParser.hpp"

// TODO: alias and root cannot be defined at the same time
// 		some variables in location block take default value from server block

using namespace std;

map<string, ServerParser::t_setter>	ServerParser::s_serverSetterMap;

void	ServerParser::setServerSetterMap()
{
	s_serverSetterMap["index"] = &ServerParser::setIndex;
	s_serverSetterMap["server_name"] = &ServerParser::setServerNames;
	s_serverSetterMap["error_code"] = &ServerParser::setErrorCode;
	s_serverSetterMap["error_page"] = &ServerParser::setErrorPage;
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
	server.m_locationTable[locationPath] = location;
	m_tokenizer.eat("}");
}

// setters
void
ServerParser::setIndex(VirtualServer& server)
{
	server.m_index = m_tokenizer.get();
	m_tokenizer.eat(";");
}

// TODO: apply server name rule
void
ServerParser::setServerNames(VirtualServer& server)
{
	// INFO: it would be better if token type is added for the token data.
	server.m_serverNames.clear();
	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		// if (checkServerName(m_tokenizer.peek()) == false) check "."
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
		if (addrPart > 255 || ss.eof() == false || ss.fail() == true)
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
	server.m_addrKey = Util::convertAddrKey(addr, port);
	LOG(Logger::DEBUG, "addr/port : %s", Util::getFormattedAddress(addr, port).c_str());
	m_tokenizer.eat(";");
}

void
ServerParser::setRoot(VirtualServer& server)
{
	server.m_root = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorPage(VirtualServer& server)
{
	while (m_tokenizer.empty() == false && m_tokenizer.peek() != ";")
	{
		server.m_errorPage.push_back(m_tokenizer.get());
	}
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
