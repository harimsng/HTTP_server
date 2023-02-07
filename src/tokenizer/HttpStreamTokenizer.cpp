#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

using namespace	std;

const char*	g_CRLF = "\r\n";

// for test

// deleted
HttpStreamTokenizer::HttpStreamTokenizer(const HttpStreamTokenizer& tokenizer)
:	ITokenizer()
{
	(void)tokenizer;
}

HttpStreamTokenizer&
HttpStreamTokenizer::operator=(const HttpStreamTokenizer& tokenizer)
{
	(void)tokenizer;
	return *this;
}

// constructors & destructor
HttpStreamTokenizer::HttpStreamTokenizer()
:	m_start(0),
	m_cur(0),
	m_end(0)
{
}

HttpStreamTokenizer::~HttpStreamTokenizer()
{
}

void
HttpStreamTokenizer::init(string& buffer)
{
	m_buffer = &buffer;
}

string::size_type
HttpStreamTokenizer::updateBuffer()
{
	string::size_type	pos;

	pos = m_buffer->find(g_CRLF);
	if (pos == string::npos)
		return string::npos;

	m_cur = 0;
	m_end = pos + 2;
	return m_end;
}

void
HttpStreamTokenizer::flush()
{
	m_buffer->erase(0, m_cur);
	m_start = 0;
	m_cur = 0;
	m_aheadToken = "";
}

std::string
HttpStreamTokenizer::peek()
{
	if (m_aheadToken.size() == 0)
		return get();
	return m_aheadToken;
}

std::string
HttpStreamTokenizer::get()
{
	string::size_type	pos;

	pos = m_buffer->find(g_CRLF);
	m_cur = pos + 2;
	if (pos == 0)
		return (g_CRLF);
	return (m_buffer->substr(0, m_cur - 2));
}

std::string
HttpStreamTokenizer::getline()
{
	return get();
}

char
HttpStreamTokenizer::getc()
{
	return empty() == false ? (*m_buffer)[m_cur++] : '\0';
}

bool
HttpStreamTokenizer::empty() const
{
	return m_cur >= m_end;
}

