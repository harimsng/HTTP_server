#ifndef SOCKETSTREAMTOKENIZER_HPP
#define SOCKETSTREAMTOKENIZER_HPP

#include "tokenizer/ITokenizer.hpp"

class	SocketStreamTokenizer: public ITokenizer
{
// deleted
	SocketStreamTokenizer(const SocketStreamTokenizer& tokenizer);
	SocketStreamTokenizer	&operator=(const SocketStreamTokenizer& tokenizer);

public:
// constructors & destructor
	SocketStreamTokenizer();
	virtual ~SocketStreamTokenizer();

// member functions
	void	init(std::string* buffer);
	virtual const std::string&	peek();
	virtual const std::string&	get();
	virtual bool				empty() const;

private:
	std::string*	m_buffer;
};

#endif
