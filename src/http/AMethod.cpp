#include "Webserv.hpp"
#include "AMethod.hpp"
#include "http/RequestHandler.hpp"

// constructors & destructor
AMethod::AMethod(RequestHandler& requestHandler)
:	m_requestHandler(requestHandler),
	m_request(m_requestHandler.m_request),
	m_sendBuffer(m_requestHandler.m_sendBuffer),
	m_recvBuffer(m_requestHandler.m_recvBuffer)
{
}

AMethod::~AMethod()
{
}


// operators
AMethod&
AMethod::operator=(const AMethod& aMethod)
{
	(void)aMethod;
	return *this;
}

// member functions
void
AMethod::completeResponse()
{
}

void
AMethod::readFile(std::string& readBody)
{
	std::fstream	file;
	std::string		readLine;

	std::string filePath = m_request.m_path + m_request.m_file;

	file.open(filePath);
	while (!file.eof())
	{
		std::getline(file, readLine);
		if (readLine == "")
			continue;
		readBody += readLine + "\n";
	}
	readBody.pop_back();
	file.close();
}


bool
AMethod::checkFileExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFREG))
		return (true);
	return (false);
}

bool
AMethod::checkDirExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFDIR))
		return (true);
	return (false);

}

void
AMethod::endResponse()
{
	m_requestHandler.resetStates();
}
