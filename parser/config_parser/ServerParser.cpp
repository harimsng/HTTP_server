#include "ConfigParser.hpp"
#include "Server.hpp"
#include "ServerParser.hpp"
#include "LocationParser.hpp"

using namespace std;

map<string, ServerParser::t_setter>	ServerParser::s_serverSetterMap;

void	ServerParser::setServerSetterMap()
{
	s_serverSetterMap["index"] = &ServerParser::setIndex;
	s_serverSetterMap["server_name"] = &ServerParser::setServerName;
	s_serverSetterMap["error_code"] = &ServerParser::setErrorCode;
	s_serverSetterMap["error_path"] = &ServerParser::setErrorPath;
	s_serverSetterMap["root"] = &ServerParser::setRoot;
	s_serverSetterMap["listen"] = &ServerParser::setListenPort;
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
ServerParser::setServerName(Server& server)
{
	server.m_serverName = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setListenPort(Server& server)
{
	server.m_listen = ConfigParser::toInt(m_tokenizer.get());
	m_tokenizer.eat(";");
}

void
ServerParser::setRoot(Server& server)
{
	server.m_root = m_tokenizer.get();
	m_tokenizer.eat(";");
}

void
ServerParser::setErrorPath(Server& server)
{
	server.m_errorPath = m_tokenizer.get();
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
