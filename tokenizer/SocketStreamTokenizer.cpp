#include "SocketStreamTokenizer.hpp"

using namespace	std;

// deleted
SocketStreamTokenizer::SocketStreamTokenizer(const SocketStreamTokenizer& tokenizer)
{
	(void)tokenizer;
}

SocketStreamTokenizer&
SocketStreamTokenizer::operator=(const SocketStreamTokenizer& tokenizer)
{
	(void)tokenizer;
	return *this;
}

// constructors & destructor
SocketStreamTokenizer::SocketStreamTokenizer()
{
}

SocketStreamTokenizer::~SocketStreamTokenizer()
{
}

void
SocketStreamTokenizer::init(string& buffer)
{
	m_buffer = &buffer;
}

bool
SocketStreamTokenizer::initBuffer(string::size_type start)
{
	string::size_type	pos;

	pos = m_buffer->rfind("\r\n", start);
	if (pos == string::npos)
		return false;
	m_start = start;
	m_cur = m_start;
	m_end = m_start + pos + 2;
	return true; 
}

const std::string&
SocketStreamTokenizer::peek()
{
	return m_token;
}

const std::string&
SocketStreamTokenizer::get()
{
	string::size_type	pos = m_buffer->find("\r\n", m_start);

	if (m_start == m_cur)
	{
		m_token = m_buffer->substr(m_start, pos - m_start);
		m_cur = pos + 2;
	}
	else
		m_prevToken = m_token;
	if (empty() == true)
	{
		m_token = "";
		return m_token;
	}
	return m_prevToken;
}

char
SocketStreamTokenizer::getc()
{
	return empty() == false ? (*m_buffer)[m_cur++] : '\0';
}

bool
SocketStreamTokenizer::empty() const
{
	return m_cur >= m_end;
}
