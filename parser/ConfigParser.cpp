#include <sstream>

#include "ServerParser.hpp"
#include "ConfigParser.hpp"
#include "FileTokenizer.hpp"

using namespace std;

// constructors & destructor
ConfigParser::ConfigParser()
{
}

ConfigParser::~ConfigParser()
{
}

void
ConfigParser::init(string configPath)
{
	tokenizer.init(configPath);
}

void
ConfigParser::parse(vector<Server>& output)
{
	while (tokenizer.get() == "server")
	{
		parseServer(output);
	}
}

void
ConfigParser::parseServer(vector<Server>& output)
{
	tokenizer.eat("{");
	
	ServerParser	serverParser(tokenizer);

	output.push_back(Server());
	serverParser.parse(output.back());
	tokenizer.eat("}");
}

int
ConfigParser::toInt(const string& str)
{
	stringstream	ss(str);
	int				num = 0;

	ss >> num;
	return num;
}
