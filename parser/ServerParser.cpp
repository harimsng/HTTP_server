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
ServerParser::ServerParser(FileTokenizer& tokenizer)
:	m_tokenizer(tokenizer)
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
		(this->*s_serverSetterMap[m_tokenizer.peek()])(server);
	}
}

void
ServerParser::parseLocation(Server& server)
{
	tokenizer.eat("{");

	LocationParser	locationParser(tokenizer);

	server.m_locationList.push_back(Location());
	locationParser.parse(server.m_locationList.back());
	tokenizer.eat("}");
}

// setters
void
ServerParser::setIndex(Server& server)
{
	server.m_index = m_tokenizer.get();
}

void
ServerParser::setServerName(Server& server)
{
	server.m_serverName = m_tokenizer.get();
}

void
ServerParser::setListenPort(Server& server)
{
	server.m_listen = ConfigParser::toInt(m_tokenizer.get());
}

void
ServerParser::setRoot(Server& server)
{
	server.m_root = m_tokenizer.get();
}

void
ServerParser::setErrorPath(Server& server)
{
	server.m_errorPath = m_tokenizer.get();
}

void
ServerParser::setErrorCode(Server& server)
{
	server.m_errorCode = m_tokenizer.get();
}

void
ServerParser::setClientMaxBodySize(Server& server)
{
	server.m_clientMaxBodySize = ConfigParser::toInt(m_tokenizer.get());
}

void
ServerParser::setUriBufferSize(Server& server)
{
	server.m_uriBufferSize = ConfigParser::toInt(m_tokenizer.get());
}
