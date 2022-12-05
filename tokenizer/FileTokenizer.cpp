#include "FileTokenizer.hpp"

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
	m_idx++;
	return (m_tokenArr[m_idx - 1]);
}

bool
FileTokenizer::empty() const
{
	return (m_idx >= m_tokenArr.size() - 1);
}

string&
FileTokenizer::getErrorLog()
{
	return m_tokenArr[m_idx];
}

void
FileTokenizer::tokenize(const string& chunk)
{
	// string	token;

	m_tokenArr.push_back(chunk);
	// for (size_t i = 0; i < chunk.size(); ++i)
	// {
    //
	// }
}

void
FileTokenizer::init(const string& path)
{
	if (!isValidFileName(path))
	{
		// throw exception;
	}

	fstream	fileStream(path.c_str());
	string	chunk;
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
	m_tokenArr.push_back("");
}

bool
FileTokenizer::isValidFileName(const string& path)
{
	struct stat	buffer;
	string		extension;
	size_t		extPos;
	int			exist;

	extPos = path.find(".");
	if (extPos == string::npos)
		return (false);
	extension = path.substr(path.find_last_of("."));
	if (extension != ".conf")
		return (false);
	exist = stat(path.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFREG))
		return (true);
	return (false);
}
