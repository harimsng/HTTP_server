#ifndef Response_hpp
#define Response_hpp

#include <ctime>
#include <string>
#include <unordered_map>

class Response
{
private:
	// std::string	m_responseMessage;

public:
	Response();
	~Response();

	void				makeResponseHeader(std::string& uffer);
	static void			setStatusCode(void);
	static const std::string	getDate(const char* format = "%a, %d %b %Y %X %Z");
	const std::string	getStatusCode(int statusCode);

	static std::unordered_map<int, std::string>	s_statusCode;
};
#endif //Response_hpp
