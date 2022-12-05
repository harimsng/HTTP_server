#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>

#include "AParser.hpp"
#include "FileTokenizer.hpp"
#include "Server.hpp"

using namespace std;

class	ConfigParser: public AParser<FileTokenizer, vector<Server> >
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
	void			init(const string& configPath);

	virtual void	parse(vector<Server>& output);

// static members
	static int		toInt(const string& str);

private:
	void			parseServer(vector<Server>& output);

// member variables
	FileTokenizer	tokenizer;

};

#endif
