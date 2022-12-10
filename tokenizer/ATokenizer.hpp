#ifndef ATOKENIZER_HPP
#define ATOKENIZER_HPP

#include <string>
#include <vector>

class	ATokenizer
{
	ATokenizer(ATokenizer const& tokenizer);
	ATokenizer	&operator=(ATokenizer const& tokenizer);

public:
// constructors & destructor
	ATokenizer();
	~ATokenizer();

// member functions
	virtual const std::string&	peek() = 0;
	virtual const std::string&	get() = 0;
	virtual bool				empty() const = 0;

private:
	char				m_buffer[1024];
	unsigned int		m_size;
};

#endif
