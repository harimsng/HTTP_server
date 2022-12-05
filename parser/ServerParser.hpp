#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include <map>
#include <string>

#include "AParser.hpp"
#include "FileTokenizer.hpp"

class	Server;

class	ServerParser: public AParser<FileTokenizer, Server>
{
	ServerParser(ServerParser const& serverParser);
	ServerParser	&operator=(ServerParser const& serverParser);

public:
// constructors & destructor
	ServerParser(FileTokenizer& tokenizer);
	virtual ~ServerParser();

// member functions
	virtual void	parse(Server& server);

	void		parseLocation(Server& server);
	void		setIndex(Server& server);
	void		setServerName(Server& server);
	void		setListenPort(Server& server);
	void		setRoot(Server& server);
	void		setErrorPath(Server& server);
	void		setErrorCode(Server& server);
	void		setClientMaxBodySize(Server& server);
	void		setUriBufferSize(Server& server);

// member variables
	FileTokenizer&		m_tokenizer;

// static members
	typedef void	(ServerParser::*t_setter)(Server&);

	static std::map<std::string, t_setter>	s_serverSetterMap;
	static void								setServerSetterMap();
};

#endif
