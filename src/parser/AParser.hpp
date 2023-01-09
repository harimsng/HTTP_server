#ifndef APARSER_HPP
#define APARSER_HPP

#include <stdexcept>

template <typename Tokenizer, typename Type>
class	AParser
{
	AParser(AParser const& aParser);
	AParser	&operator=(AParser const& aParser);

protected:
// constructors & destructor
	AParser() {};
	virtual ~AParser() {};

// member functions
	virtual void	parse(Type& type) = 0;

// member variables
	Tokenizer	m_tokenizer;
};

template <typename Tokenizer>
class	AParser<Tokenizer, void>
{
	AParser(AParser const& aParser);
	AParser	&operator=(AParser const& aParser);

protected:
// constructors & destructor
	AParser() {};
	virtual ~AParser() {};

// member functions
	virtual void	parse(void) = 0;

// member variables
	Tokenizer	m_tokenizer;
};

#endif
