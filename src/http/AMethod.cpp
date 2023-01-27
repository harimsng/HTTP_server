#include "http/RequestHandler.hpp"
#include "io/Buffer.hpp"
#include "util/Util.hpp"
#include "AMethod.hpp"

// constructors & destructor
AMethod::AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
	:m_request(request),
	m_sendBuffer(sendBuffer),
	m_recvBuffer(recvBuffer)
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
