#include <sys/stat.h>

#include <iostream>

#include "Webserv.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "exception/ConfigParserException.hpp"
#include "parser/ServerParser.hpp"
#include "parser/LocationParser.hpp"
#include "parser/ConfigParser.hpp"

using namespace std;

// constructors & destructor
ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

void
ConfigParser::init(string configPath, VirtualServerTable& serverTable)
{
	ServerParser::setServerSetterMap();
	LocationParser::setLocationSetterMap();

	if (checkFileStat(configPath.data()) == false)
		throw ConfigParserException("invalid file type");

	if (configPath.rfind(".conf") != configPath.size() - 5)
		throw ConfigParserException("invalid filename");

	m_serverTable = &serverTable;
	m_tokenizer.init(configPath);
}

bool
ConfigParser::checkFileStat(const char* path)
{
	struct stat	buffer;

	if (stat(path, &buffer) == -1)
		throw ConfigParserException("file doesn't exist");
	return (buffer.st_mode & S_IFREG) == S_IFREG;
}

void
ConfigParser::parse() try
{
	while (m_tokenizer.peek() == "server")
	{
		m_tokenizer.get();
		parseServer();
	}
	if (m_tokenizer.empty() == false)
		m_tokenizer.eat("server");
}
catch (exception& e)
{
	cout << m_tokenizer.getErrorLog(e.what());
}

void
ConfigParser::parseServer() throw(string) try
{
	m_tokenizer.eat("{");
	
	ServerParser	serverParser(m_tokenizer);

	Server	newServer;
	serverParser.parse(newServer);
	m_tokenizer.eat("}");

	if (m_serverTable->count(newServer.m_addrKey) == 1)
		checkDuplicateServerName(newServer);
	else
		(*m_serverTable)[newServer.m_addrKey];
	addNameToTable(newServer);
}
catch (string& serverName)
{
}

void
ConfigParser::checkDuplicateServerName(const Server& server) const
{
	ServerNameTable&		table = (*m_serverTable)[server.m_addrKey];
	const vector<string>&	names = server.m_serverNames;

	for (size_t i = 0; i < names.size(); ++i)
	{
		if (table.count(names[i]) == 1)
			throw names[i];
	}
}

void
ConfigParser::addNameToTable(Server& server)
{
	ServerNameTable&		table = (*m_serverTable)[server.m_addrKey];
	const vector<string>&	names = server.m_serverNames;

	for (size_t i = 0; i < names.size(); ++i)
	{
		table[names[i]];
	}
}
