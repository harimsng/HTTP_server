#include "http/AMethod.hpp"
#include "HeadMethod.hpp"

// constructors & destructor

HeadMethod::HeadMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
{
}

HeadMethod::~HeadMethod()
{
}

// operators
HeadMethod&
HeadMethod::operator=(const HeadMethod& headMethod)
{
	(void)headMethod;
	return *this;
}

void
HeadMethod::completeResponse()
{
	/*
	std::string filePath = m_request.m_path + m_request.m_file;

	if (!checkFileExists(filePath))
	{
		filePath = m_conf.getErrorPath();
		m_statusCode = 404;
		m_filePath.replace(m_filePath.find('*'), 1, std::to_string(m_statusCode));
		return ;
	}
	*/
}

