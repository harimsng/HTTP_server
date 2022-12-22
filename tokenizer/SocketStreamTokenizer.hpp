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
	void	init(std::string& buffer);
	bool	initBuffer(size_t start = 0);
	virtual const std::string&	peek();
	virtual const std::string&	get();
	char			getc();
	virtual bool	empty() const;

private:
	std::string*	m_buffer;
	std::string		m_token;
	std::string		m_prevToken;
	std::string::size_type	m_start;
	std::string::size_type	m_cur;
	std::string::size_type	m_end;
};

#endif
