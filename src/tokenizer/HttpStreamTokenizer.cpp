#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

using namespace	std;

const char*	g_CRLF = "\r\n";

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
HttpStreamTokenizer::updateBuffer()
{
	string::size_type	pos;

	flush();
	pos = m_buffer->rfind(g_CRLF);
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
	string::size_type	pos = m_buffer->find(g_CRLF, m_cur);
	string				token = m_aheadToken;

	if (empty() == true)
		return "";

	pos = pos == string::npos ? m_buffer->size() : pos;
	m_aheadToken = m_buffer->substr(m_cur, pos - m_cur);
	m_cur = pos + 2;

	//  if token.size() == 0 (m_aheadToken was empty) current call to this method is first call.
	if (token.size() == 0)
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

void
HttpStreamTokenizer::flush()
{
	m_buffer->erase(0, m_cur);
	m_start = 0;
	m_cur = 0;
	m_end = m_buffer->size();
	m_aheadToken = "";
}

bool
HttpStreamTokenizer::empty() const
{
	return m_cur >= m_end;
}

