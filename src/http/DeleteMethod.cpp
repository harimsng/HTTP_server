#include "DeleteMethod.hpp"

using namespace std;

// constructors & destructor
DeleteMethod::DeleteMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
: AMethod(request, sendBuffer, recvBuffer)
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
	vector<string> limitExcept;
	istringstream ss(m_request.m_locationBlock.m_limitExcept);
	string	token;

	string filePath = m_request.m_path + m_request.m_file;

	while (getline(ss, token, ',')) // method 예외 목록 저장
	{
        limitExcept.push_back(token);
    }

	if (!checkFileExists(filePath))
	{
		filePath = m_conf.getErrorPath(); // error 페이지 가져오는 코드
		m_statusCode = 404;
		filePath.replace(filePath.find('*'), 1, to_string(m_statusCode)); // error 페이지 가져오는 코드
		return ;
	}
	if (!this->checkMethodLimit(limitExcept))
	{
		filePath = m_conf.getErrorPath();
		m_statusCode = 405;
		filePath.replace(filePath.find('*'), 1, to_string(m_statusCode));
	}
	readFile(m_sendBuffer); // 파일 읽기
	deleteStatus = unlink(filePath.c_str());
	if (deleteStatus == -1)
		m_statusCode = 202;
	*/
}

