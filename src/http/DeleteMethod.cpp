#include "DeleteMethod.hpp"

using namespace std;

// constructors & destructor
DeleteMethod::DeleteMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
{
}

DeleteMethod::~DeleteMethod()
{
}

// operators
DeleteMethod&
DeleteMethod::operator=(const DeleteMethod& deleteMethod)
{
	(void)deleteMethod;
	return *this;
}

bool
DeleteMethod::checkMethodLimit(const vector<string>& limitExcept) const
{
	for (size_t i = 0; i < limitExcept.size(); i++)
	{
		if (limitExcept[i] == "DELETE")
			return (true);
	}
	return (false);
}

void
DeleteMethod::completeResponse()
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

