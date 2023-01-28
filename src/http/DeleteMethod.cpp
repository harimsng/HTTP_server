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

	if (!checkFileExists(filePath))
	{
		filePath = m_conf.getErrorPath(); // error 페이지 가져오는 코드
		m_statusCode = 404;
		filePath.replace(filePath.find('*'), 1, to_string(m_statusCode)); // error 페이지 가져오는 코드
		return ;
	}

	readFile(m_sendBuffer); // 파일 읽기
	deleteStatus = unlink(filePath.c_str());
	if (deleteStatus == -1)
		m_statusCode = 202;
	*/
}

