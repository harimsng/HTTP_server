#include <sstream>
#include <iostream>

#include "parser/config_parser/ConfigParser.hpp"
#include "tokenizer/FileTokenizer.hpp"
#include "exception/FileTokenizerException.hpp"

using namespace std;

const std::string	FileTokenizer::s_eot = "";

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
	if (empty() == true)
		return s_eot;
	return (m_tokenArr[m_idx].first);
}

const string&
FileTokenizer::get()
{
	if (empty() == true)
		throw FileTokenizerException("consumed every tokens");
	return (m_tokenArr[m_idx++].first);
}

void
FileTokenizer::eat(const string& target)
{
	if (get() != target)
		throw FileTokenizerException("unexpected token");
}

unsigned int
FileTokenizer::size()
{
	return m_tokenArr.size();
}

bool
FileTokenizer::empty() const
{
	return m_idx >= m_tokenArr.size();
}

string
FileTokenizer::getErrorLog(const char* message)
{
	int		errorLineNumber = m_tokenArr[m_idx - 1].second;
	string	errorLine;
	string	errorLog;

	m_fstream.clear();
	m_fstream.seekg(ios_base::beg);
	cout << message << '\n';
	for (int i = 1; i < errorLineNumber - 2; ++i)
		getline(m_fstream, errorLine);

	if (errorLineNumber >= 3)
	{
		getline(m_fstream, errorLine);
		errorLine = "line " + ConfigParser::toString(errorLineNumber - 2) + ": " + errorLine;
		errorLog += errorLine + "\n";
	}
	if (errorLineNumber >= 2)
	{
		getline(m_fstream, errorLine);
		errorLine = "line " + ConfigParser::toString(errorLineNumber - 1) + ": " + errorLine;
		errorLog += errorLine + "\n";
	}
	getline(m_fstream, errorLine);
	errorLine = "line " + ConfigParser::toString(errorLineNumber) + ": " + errorLine;
	errorLog += errorLine + "  <<\n";
	return errorLog;
}

void
FileTokenizer::tokenize(const std::string& chunk, int lineNumber)
{
	for (string::size_type start = 0, end = 0; end < chunk.size(); ++end)
	{
		if (chunk[end] == ';' || chunk[end] == '{' || chunk[end] == '}')
		{
			if (start != end)
				m_tokenArr.push_back(make_pair(chunk.substr(start, end - start), lineNumber));
			m_tokenArr.push_back(make_pair(string(1, chunk[end]), lineNumber));
			start = end + 1;
		}
		else if (chunk[end] == '\0')
		{
			if (start != end)
				m_tokenArr.push_back(make_pair(chunk.substr(start, end - start), lineNumber));
		}
	}
}

void
FileTokenizer::init(const string path)
{
	string		line;

	m_fstream.open(path);
	for (int lineNumber = 1; m_fstream.good() == true; ++lineNumber)
	{
		if (std::getline(m_fstream, line).good() == false)
			break;
		if (line == "")
			continue;

		stringstream	ss(line);
		string			chunk;

		while (ss.good())
		{
			ss >> chunk;
			chunk.push_back('\0');
			tokenize(chunk, lineNumber);
		}
	}
	if (m_fstream.eof() == false && m_fstream.fail())
		throw FileTokenizerException("file read error");
	m_idx = 0;
}

void
FileTokenizer::printTokens() const
{
	for (unsigned int i = 0; i < m_tokenArr.size(); ++i)
		cout << "\'" << m_tokenArr[i].first << "'\n";
}

void
FileTokenizer::printTokensByLine() const
{
	int		line = 1;

	cout << "line 1: ";
	for (unsigned int i = 0; i < m_tokenArr.size(); ++i)
	{
		if (line != m_tokenArr[i].second)
		{
			++line;
			cout << "\nline " + ConfigParser::toString(line) + ": ";
		}
		cout << m_tokenArr[i].first << ' ';
	}
	cout << "\n";
}
