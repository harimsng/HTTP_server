#ifndef LOCATIONPARSER_HPP
#define LOCATIONPARSER_HPP

#include <map>
#include <string>

#include "parser/AParser.hpp"
#include "tokenizer/FileTokenizer.hpp"

class	Location;

class	LocationParser: public AParser<FileTokenizer, Location>
{
// deleted
	LocationParser(LocationParser const& locationParser);
	LocationParser	&operator=(LocationParser const& locationParser);

// types
	typedef void	(LocationParser::*t_setter)(Location&);

public:
// constructors & destructor
	LocationParser(FileTokenizer& tokenizer);
	virtual ~LocationParser();

// member functions
	virtual void	parse(Location& location);

	void		setIndex(Location& location);
	void		setAutoIndex(Location& location);
	void		setLimitExcept(Location& location);
	void		setPath(Location& location);
	void		setRoot(Location& location);
	void		setAlias(Location& location);
	void		setClientMaxBodySize(Location& location);
	void		setErrorPage(Location& location);
	void		setCgiPass(Location& location);
	void		setCgiExt(Location& location);

private:
// member variables
	FileTokenizer&		m_tokenizer;

public:
// static members
	static std::map<std::string, t_setter>	s_locationSetterMap;
	static void								setLocationSetterMap();
};

#endif
