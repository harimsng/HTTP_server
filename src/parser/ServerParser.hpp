#ifndef SERVERPARSER_HPP
#define SERVERPARSER_HPP

#include <map>
#include <string>

#include "parser/AParser.hpp"
#include "tokenizer/FileTokenizer.hpp"

class	VirtualServer;

class	ServerParser: public AParser<FileTokenizer, VirtualServer>
{
	ServerParser(ServerParser const& serverParser);
	ServerParser	&operator=(ServerParser const& serverParser);

public:
// static members
	typedef void	(ServerParser::*t_setter)(VirtualServer&);

	static std::map<std::string, t_setter>	s_serverSetterMap;
	static void								setServerSetterMap();

// constructors & destructor
	ServerParser(FileTokenizer& tokenizer);
	virtual ~ServerParser();

// member functions
	virtual void	parse(VirtualServer& server);

	void		parseLocation(VirtualServer& server);
	void		setIndex(VirtualServer& server);
	void		setServerNames(VirtualServer& server);
	void		setListenAddress(VirtualServer& server);
	void		setRoot(VirtualServer& server);
	void		setErrorPages(VirtualServer& server);
	void		setErrorCode(VirtualServer& server);
	void		setClientMaxBodySize(VirtualServer& server);
	void		setUriBufferSize(VirtualServer& server);

private:
// member variables
	FileTokenizer&		m_tokenizer;

};

#endif
