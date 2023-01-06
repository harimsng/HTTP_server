#ifndef HTTPSTREAMTOKENIZER_HPP
#define HTTPSTREAMTOKENIZER_HPP

#include "tokenizer/ITokenizer.hpp"

class	HttpStreamTokenizer: public ITokenizer
{
// deleted
	HttpStreamTokenizer(const HttpStreamTokenizer& tokenizer);
	HttpStreamTokenizer
	&operator=(const HttpStreamTokenizer& tokenizer);

public:
// constructors & destructor
	HttpStreamTokenizer();
	virtual ~HttpStreamTokenizer();

// member functions
	void	init(std::string& buffer);
	std::string::size_type	updateBufferForHeader();
	std::string::size_type	updateBufferForBody();

	// should those virtual method be private?
	virtual std::string	peek();
	virtual std::string	get();
	std::string			getline();
	char			getc();
	virtual bool	empty() const;

private:
	std::string*	m_buffer;
	std::string		m_aheadToken;

	std::string::size_type	m_start;
	std::string::size_type	m_cur;
	std::string::size_type	m_end;
};

#endif
