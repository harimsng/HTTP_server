#include "exception/HttpErrorHandler.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

using namespace	std;

// for test
#include <iostream>

// deleted
HttpStreamTokenizer::HttpStreamTokenizer(const HttpStreamTokenizer& tokenizer)
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

	pos = m_buffer->rfind("\r\n");
	if (pos == string::npos)
	{
		if (m_buffer->size() == m_buffer->capacity())
			throw HttpErrorHandler(501);

		return string::npos;
	}
	m_cur = m_start;
	m_end = m_start + pos + 2;
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

const std::string&
HttpStreamTokenizer::peek()
{
	if (m_token.size() == 0)
		return get();

	return m_aheadToken;
}

/* NOTE
 * this method is supposed to return token,
 * but it's not the one defined in http rfc.
 * @return a line terminated with \r\n.
 */
const std::string&
HttpStreamTokenizer::get()//line()?
{
	string::size_type	pos = m_buffer->find("\r\n", m_cur);
	string::size_type	temp;

	m_token = m_aheadToken;
	if (empty() == true)
	{
		m_aheadToken = "";
		return m_token;
	}
	pos = pos == string::npos ? m_buffer->size() : pos;
	m_aheadToken = m_buffer->substr(m_cur, pos - m_cur);
	temp = m_cur;
	m_cur = pos + 2;
	if (m_start == temp)
		get();

	return m_token;
}

const std::string&
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

