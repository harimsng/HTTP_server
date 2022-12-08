#include "FileTokenizer.hpp"

using namespace std;

// constructors & destructor
FileTokenizer::FileTokenizer()
{
}

FileTokenizer::~FileTokenizer()
{
}

const string&
FileTokenizer::peek()
{
	return (m_tokenArr[m_idx]);
}

const string&
FileTokenizer::get()
{
	if (peek() == "")
	{
		// throw exception
	}
	return (m_tokenArr[m_idx++]);
}

bool
FileTokenizer::empty() const
{
	return m_idx >= m_tokenArr.size() - 1;
}

string&
FileTokenizer::getErrorLog()
{
	return m_tokenArr[m_idx];
}

void
FileTokenizer::tokenize(const std::string& chunk)
{
	string	token;
	
	for (string::size_type i = 0; i < chunk.size(); ++i)
	{
		
	}
}

void
FileTokenizer::init(const std::string path)
{
	size_t extPos = path.find(".");
	if (extPos == std::string::npos)
	{
		// throw exception;
	}

	std::string extension = path.substr(path.find_last_of("."));
	if (extension != ".conf")
	{
		//throw exception;
	}

	std::fstream	fileStream(path.c_str());
	std::string		chunk;
	while (fileStream.good() == true)
	{
		fileStream >> chunk;
		tokenize(chunk);
	}
	if (fileStream.fail())
	{
		// throw exception;
	}
	m_idx = 0;
}

