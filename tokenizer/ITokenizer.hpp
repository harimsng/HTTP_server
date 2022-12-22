#ifndef ITOKENIZER_HPP
#define ITOKENIZER_HPP

#include <string>
#include <vector>

class	ITokenizer
{
	ITokenizer(ITokenizer const& tokenizer);
	ITokenizer	&operator=(ITokenizer const& tokenizer);

protected:
// constructors & destructor
	ITokenizer() {};
	virtual ~ITokenizer() {};

// member functions
	virtual const std::string&	peek() = 0;
	virtual const std::string&	get() = 0;
	virtual bool				empty() const = 0;
};

#endif
