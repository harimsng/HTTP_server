#include "SocketStreamTokenizer.hpp"

// constructors & destructor
SocketStreamTokenizer::SocketStreamTokenizer()
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
