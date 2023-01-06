#ifndef APARSER_HPP
#define APARSER_HPP

#include <stdexcept>

template <typename Tokenizer, typename OutputType>
class	AParser
{
	AParser(AParser const& aParser);
	AParser	&operator=(AParser const& aParser);

protected:
// constructors & destructor
	AParser() {};
	virtual ~AParser() {};

// member functions
	virtual void	parse(OutputType& output) = 0;

// member variables
	Tokenizer	m_tokenizer;
};

#endif
