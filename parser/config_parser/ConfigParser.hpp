#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>

#include "AParser.hpp"
#include "FileTokenizer.hpp"
#include "Server.hpp"

class	ConfigParser: public AParser<FileTokenizer, std::vector<Server> >
{
// deleted
	ConfigParser(ConfigParser const& configParser);
	ConfigParser	&operator=(ConfigParser const& configParser);

	class	ConfigParserException;

public:
// constructors & destructor
	ConfigParser();
	virtual ~ConfigParser();

// member functions
	void			init(std::string configPath);
	bool			checkFileStat(const char* path);

	virtual void	parse(std::vector<Server>& output);
	void			parseServer(std::vector<Server>& output);

// static members
	static int		toInt(const std::string& str);
	static std::string		toString(int num);
};

#endif
