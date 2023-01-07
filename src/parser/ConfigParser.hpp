#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>

#include "Webserv.hpp"
#include "parser/AParser.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "Server.hpp"

class	ConfigParser: public AParser<FileTokenizer, std::vector<Server> >
{
// deleted
	ConfigParser(ConfigParser const& configParser);
	ConfigParser	&operator=(ConfigParser const& configParser);

public:
	class	ConfigParserException;

// constructors & destructor
	ConfigParser();
	virtual ~ConfigParser();

// member functions
	void			init(std::string configPath);
	bool			checkFileStat(const char* path);

	virtual void	parse(std::vector<Server>& servers);
	void			parseServer(std::vector<Server>& servers);

private:
	AddrResolutionTable*	m_addrResolutionTable;

public:
// static members
	static int			toInt(const std::string& str);
	static std::string	toString(int num);
};

#endif
