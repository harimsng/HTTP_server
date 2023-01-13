#ifndef LOCATIONPARSER_HPP
#define LOCATIONPARSER_HPP

#include <map>
#include <string>

#include "parser/AParser.hpp"
#include "tokenizer/FileTokenizer.hpp"

class	Location;

class	LocationParser: public AParser<FileTokenizer, Location>
{
	LocationParser(LocationParser const& locationParser);
	LocationParser	&operator=(LocationParser const& locationParser);

public:
// static members
	typedef void	(LocationParser::*t_setter)(Location&);

	static std::map<std::string, t_setter>	s_locationSetterMap;
	static void								setLocationSetterMap();

// constructors & destructor
	LocationParser(FileTokenizer& tokenizer);
	virtual ~LocationParser();

// member functions
	virtual void	parse(Location& location);

	void		setIndex(Location& location);
	void		setAutoIndex(Location& location);
	void		setExpires(Location& location);
	void		setProxyPass(Location& location);
	void		setLimitExcept(Location& location);
	void		setPath(Location& location);
	void		setRoot(Location& location);
	void		setCgiPass(Location& location);
	void		setAlias(Location& location);
	void		setClientMaxBodySize(Location& location);

private:
// member variables
	FileTokenizer&		m_tokenizer;
};

#endif
