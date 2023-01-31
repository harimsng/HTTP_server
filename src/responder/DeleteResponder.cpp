#include "DeleteResponder.hpp"

using namespace std;

// constructors & destructor
DeleteResponder::DeleteResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

DeleteResponder::~DeleteResponder()
{
}

// operators
DeleteResponder&
DeleteResponder::operator=(const DeleteResponder& deleteMethod)
{
	(void)deleteMethod;
	return *this;
}

void
DeleteResponder::respond()
{
	/*
	int deleteStatus;

	string filePath = m_request.m_path + m_request.m_file;

	readFile(m_sendBuffer); // 파일 읽기
	deleteStatus = unlink(filePath.c_str());
	if (deleteStatus == -1)
		m_statusCode = 202;
	*/
}
