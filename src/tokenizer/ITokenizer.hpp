#ifndef ITOKENIZER_HPP
#define ITOKENIZER_HPP

#include <string>

class	ITokenizer
{
	ITokenizer(ITokenizer const& tokenizer);
	ITokenizer	&operator=(ITokenizer const& tokenizer);

protected:
// constructors & destructor
	ITokenizer() {};
	virtual ~ITokenizer() {};

// member functions
	virtual std::string	peek() = 0;
	virtual std::string	get() = 0;
	virtual bool		empty() const = 0;
};

#endif
