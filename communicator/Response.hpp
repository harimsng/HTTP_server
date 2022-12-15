#ifndef Response_hpp
#define Response_hpp

#include <ctime>
#include <string>
#include <map>

class Response
{
private:
	// std::string	m_responseMessage;

public:
	Response();
	~Response();

	void				makeResponseHeader(std::string& uffer);
	static void			setStatusMessageTable(void);
	const std::string	getStatusMessage(int statusCode);

	static std::map<int, std::string>	s_statusMessageTable;
};
#endif //Response_hpp
