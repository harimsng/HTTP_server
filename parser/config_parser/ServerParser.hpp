#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include <map>
#include <string>

#include "parser/AParser.hpp"
#include "tokenizer/FileTokenizer.hpp"

class	Server;

class	ServerParser: public AParser<FileTokenizer, Server>
{
	ServerParser(ServerParser const& serverParser);
	ServerParser	&operator=(ServerParser const& serverParser);

public:
// static members
	typedef void	(ServerParser::*t_setter)(Server&);

	static std::map<std::string, t_setter>	s_serverSetterMap;
	static void								setServerSetterMap();

// constructors & destructor
	ServerParser(FileTokenizer& tokenizer);
	virtual ~ServerParser();

// member functions
	virtual void	parse(Server& server);

	void		parseLocation(Server& server);
	void		setIndex(Server& server);
	void		setServerNames(Server& server);
	void		setListenAddress(Server& server);
	void		setRoot(Server& server);
	void		setErrorPages(Server& server);
	void		setErrorCode(Server& server);
	void		setClientMaxBodySize(Server& server);
	void		setUriBufferSize(Server& server);

private:
// member variables
	FileTokenizer&		m_tokenizer;

};

#endif
