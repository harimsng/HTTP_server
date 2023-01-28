#include "Cgi.hpp"
// deleted
Cgi&	Cgi::operator=(Cgi const& cgi)
{
	(void)cgi;
	return *this;
}

// constructors & destructor
Cgi::Cgi()
{
    m_env.clear();
    m_envChar.clear();
    m_cgiPath = "";
    m_path = "";
    m_script = "";
    m_query = "";
	m_bodyFlag = false;
}

Cgi::~Cgi()
{
}

Cgi::Cgi(Cgi const& cgi)
{
	(void)cgi;
}

void
Cgi::initCgi(const Request &request)
{
	m_cgiPath = request.m_locationBlock.m_scgiPass;
	m_path = request.m_path + request.m_file;

    std::string CONTENT_LENGTH = "CONTENT_LENGTH=";
    std::string CONTENT_TYPE = "CONTENT_TYPE=";
    std::map<std::string, std::vector<std::string> >::const_iterator contentIt;
    contentIt = request.m_headerFieldsMap.find("Content-Type");
    if (contentIt != request.m_headerFieldsMap.end())
    {
        contentIt = request.m_headerFieldsMap.find("Content-Type");
        std::string type = contentIt->second[0];
        CONTENT_TYPE += type;
    }

	std::string	HTTP_X_SECRET_HEADER_FOR_TEST;
    contentIt = request.m_headerFieldsMap.find("X-Secret-Header-For-Test");
    if (contentIt != request.m_headerFieldsMap.end())
    {
        std::string type = contentIt->second[0];
    	HTTP_X_SECRET_HEADER_FOR_TEST += "HTTP_X_SECRET_HEADER_FOR_TEST=" + type;
    }
	/*
    if (request.m_method == RequestHandler::POST && method->getBodySize() != -1)
    {
        CONTENT_LENGTH += std::to_string(method->getBodySize());
    }
	*/
    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200"; // php-cgi direct exec
    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1"; // different GET POST
    std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE=CGI/1.1";
    std::string REQUEST_METHOD = "REQUEST_METHOD=" + request.m_method; // 실제 메소드 이름으로 수정 필요
    std::string REQUEST_URI = "REQUEST_URI=" + request.m_uri;
    std::string PATH_INFO = "PATH_INFO=" + request.m_uri;
    std::string PATH_TRANSLATED = "PATH_TRANSLATED=" + request.m_uri;
    std::string SCRIPT_NAME = "SCRIPT_NAME=" + request.m_file;
    std::string SCRIPT_FILENAME = "SCRIPT_FILENAME=" + m_path;
    std::string QUERY_STRING = "QUERY_STRING=";
    if (request.m_method == RequestHandler::GET)
    {
        QUERY_STRING += request.m_queryString;
    }
    std::string SERVER_NAME = "SERVER_NAME=";
    std::string HTTP_HOST = "HTTP_HOST=";
    std::string SERVER_PORT = "SERVER_PORT=";
    std::map<std::string, std::vector<std::string> >::const_iterator mapIt;
    mapIt = request.m_headerFieldsMap.find("Host");
    std::string port = mapIt->second[0];
    if (port.find(":") != std::string::npos)
        SERVER_PORT += port.substr(port.find(":") + 1);
    else
        SERVER_PORT += "80";
    SERVER_NAME += port;
    HTTP_HOST += port.substr(0, port.find(":"));
    std::string REMOTE_ADDR = "REMOTE_ADDR=";
    std::string REMOTE_HOST = "REMOTE_HOST=" + port;
    std::string REMOTE_IDENT = "REMOTE_IDENT=";
    std::string SERVER_SOFTWARE = "SERVER_SOFTWARE=webserv/2.0";

    m_env.push_back(REDIRECT_STATUS);
    m_env.push_back(CONTENT_LENGTH);
    m_env.push_back(CONTENT_TYPE);
    m_env.push_back(SERVER_PROTOCOL);
    m_env.push_back(GATEWAY_INTERFACE);
    m_env.push_back(REQUEST_METHOD);
    m_env.push_back(REQUEST_URI);
    m_env.push_back(PATH_INFO);
    m_env.push_back(PATH_TRANSLATED);
    m_env.push_back(SCRIPT_NAME);
    m_env.push_back(SCRIPT_FILENAME);
    m_env.push_back(QUERY_STRING);
    m_env.push_back(SERVER_NAME);
    m_env.push_back(SERVER_PORT);
    m_env.push_back(HTTP_HOST);
    m_env.push_back(REMOTE_ADDR);
    m_env.push_back(REMOTE_HOST);
    m_env.push_back(REMOTE_IDENT);
    m_env.push_back(SERVER_SOFTWARE);
	m_env.push_back(HTTP_X_SECRET_HEADER_FOR_TEST);

    for (size_t i = 0; i < m_env.size(); i++)
	{
		m_envChar.push_back(const_cast<char*>(m_env[i].c_str()));
	}
    m_envChar.push_back(0);
}

