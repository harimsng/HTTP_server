#include "SocketStreamTokenizer.hpp"

// constructors & destructor
SocketStreamTokenizer::SocketStreamTokenizer(std::string& buffer)
:	m_buffer(&buffer)
{
}

SocketStreamTokenizer::~SocketStreamTokenizer()
{
}

SocketStreamTokenizer::SocketStreamTokenizer(const SocketStreamTokenizer& tokenizer)
{
	(void)tokenizer;
}

// operators
SocketStreamTokenizer&
SocketStreamTokenizer::operator=(const SocketStreamTokenizer& tokenizer)
{
	(void)tokenizer;
	return *this;
}

void
SocketStreamTokenizer::init(std::string* buffer)
{
	m_buffer = buffer;
}
