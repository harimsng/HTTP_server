#include "HttpInfo.hpp"

using namespace std;

HttpInfo::HttpInfo(Server& server)
	:m_server(&server),
	m_requestReadStatus(REQUEST_LINE)
{
}

HttpInfo::~HttpInfo()
{
}

std::ostream&
operator<<(std::ostream& os, const HttpInfo& httpInfo)
{
	requestHeaderFields_t::const_iterator mapIt;

	os << "reqeust info\n";
	os << "\tmethod : " << httpInfo.m_method << endl;
	os << "\ttarget : " << httpInfo.m_target << endl;
	os << "\tprotocol : " << httpInfo.m_protocol << endl;
	os << "header field\n";
	for (mapIt = httpInfo.m_requestHeaderFields.begin();
			mapIt != httpInfo.m_requestHeaderFields.end(); mapIt++)
	{
		os << "\t" << mapIt->first << " : ";
		vector<string>::const_iterator vecIt = mapIt->second.begin();
		for (; vecIt != mapIt->second.end(); vecIt++)
			os << *vecIt << " ";
		os << "\n";
	}
	return (os);

}
