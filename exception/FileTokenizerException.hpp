#ifndef FILETOKENIZEREXCEPTION_HPP
#define FILETOKENIZEREXCEPTION_HPP

#include <stdexcept>
#include <string>

#include "FileTokenizer.hpp"

class	FileTokenizer::FileTokenizerException: public std::exception
{
public:
	FileTokenizerException(const std::string& message): m_message("FileTokenizerException: " + message) {};

	virtual ~FileTokenizerException() throw() {};

	virtual const char*	what() const throw()
	{
		return m_message.c_str();
	}

private:
	std::string	m_message;
};

#endif
