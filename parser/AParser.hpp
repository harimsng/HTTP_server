#ifndef APARSER_HPP
#define APARSER_HPP

#include <stdexcept>

template <typename Tokenizer, typename OutputClass>
class	AParser
{
	AParser(AParser const& aParser);
	AParser	&operator=(AParser const& aParser);

protected:
// constructors & destructor
	AParser() {};
	virtual ~AParser() {};

// member functions
	virtual void	parse(OutputClass& outputClass) = 0;

// member variables
	Tokenizer	tokenizer;
};

#endif
