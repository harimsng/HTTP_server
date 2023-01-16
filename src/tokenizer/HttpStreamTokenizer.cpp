#include "exception/HttpErrorHandler.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

using namespace	std;

// for test
#include <iostream>

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
HttpStreamTokenizer::updateBufferForHeader()
{
	string::size_type	pos;

	if (m_cur > m_start)
		m_start = m_cur;

	pos = m_buffer->rfind("\r\n\r\n");
	if (pos == string::npos)
	{
		if (m_buffer->size() == m_buffer->capacity())
			throw HttpErrorHandler(501);

		return string::npos;
	}
	m_cur = m_start;
	m_end = m_start + pos + 4;
	return m_end;
}

string::size_type
HttpStreamTokenizer::updateBufferForBody()
{
	if (m_cur > m_start)
		m_start = m_cur;

	m_cur = m_start;
	m_end = m_buffer->size();
	return m_end;
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
	string::size_type	pos = m_buffer->find("\r\n", m_cur);
	string::size_type	temp;
	string				token = m_aheadToken;

	if (empty() == true)
		return token;

	pos = pos == string::npos ? m_buffer->size() : pos;
	m_aheadToken = m_buffer->substr(m_cur, pos - m_cur);
	temp = m_cur;
	m_cur = pos + 2;
	//  m_start == temp (original value of m_cur) means it's first call to get() on new stream.
	if (m_start == temp)
		return get();
	return token;
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

