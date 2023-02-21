#include <iostream>

#include "Webserv.hpp"
#include "Logger.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "exception/ConfigParserException.hpp"
#include "util/Util.hpp"
#include "parser/ServerParser.hpp"
#include "parser/LocationParser.hpp"
#include "parser/ConfigParser.hpp"

#ifndef WEBSERV_ROOT
# define WEBSERV_ROOT "/"
#endif

using namespace std;

extern const string	g_webservDir = WEBSERV_ROOT;

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

	if (Util::checkFileStat(configPath.data()) == false)
		throw ConfigParserException("invalid file type");

	if (configPath.rfind(".conf") != configPath.size() - 5)
		throw ConfigParserException("invalid file extension");

	m_serverTable = &serverTable;
	m_tokenizer.init(configPath);
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
	throw;
}

void
ConfigParser::parseServer() try
{
	m_tokenizer.eat("{");

	ServerParser	serverParser(m_tokenizer);

	VirtualServer*	newServer = new VirtualServer;
	serverParser.parse(*newServer);
	m_tokenizer.eat("}");

	if (m_serverTable->count(newServer->m_addrKey) == 1)
		checkDuplicateServerName(*newServer);
	else
		(*m_serverTable)[newServer->m_addrKey]["."] = newServer;
	addNameToTable(*newServer);
}
// TODO: a server which has duplicated servername is ignored.
catch (VirtualServer* newServer)
{
	delete newServer;
}
catch (string& duplicatedServerName)
{

}

void
ConfigParser::checkDuplicateServerName(VirtualServer& server) try
{
	ServerNameTable&		table = (*m_serverTable)[server.m_addrKey];
	const vector<string>&	names = server.m_serverNames;

	for (size_t i = 0; i < names.size(); ++i)
	{
		if (table.count(names[i]) == 1)
			throw (make_pair(names[i], &server));
	}
}
catch (pair<string, VirtualServer*> e)
{
	LOG(WARNING, "duplicated server name \"%s\" in same address", e.first.c_str());
	throw e.second;
}

void
ConfigParser::addNameToTable(VirtualServer& server)
{
	ServerNameTable&		table = (*m_serverTable)[server.m_addrKey];
	const vector<string>&	names = server.m_serverNames;

	for (size_t i = 0; i < names.size(); ++i)
	{
		table[names[i]] = &server;
	}
}
