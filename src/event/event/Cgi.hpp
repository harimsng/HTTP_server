#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>
#include <sys/stat.h>

#include "event/EventObject.hpp"
#include "io/Buffer.hpp"

class	RequestHandler;
struct	Request;

class	Cgi: public EventObject
{
	enum	e_status
	{
		CGI_HEADER = 1,
		CGI_CONTENT
	};
// deleted
	Cgi	&operator=(Cgi const& cgi);
	Cgi(Cgi const& cgi);

public:
// constructors & destructor
	Cgi(int fileFd, int writeEnd, RequestHandler& requestHandler);
	Cgi(int cgiToServer[2], int serverToCgi[2], RequestHandler& requestHandler, Buffer& toCgiBuffer);
	~Cgi();

// member functions
	void	initEnv(const Request& request);
	void	executeCgi(int pipe[2], std::string& readBody, const Request &request);
// #ifdef TEST
	void	executeCgi();
// #endif
	int		receiveCgiResponse();
	int		sendCgiRequest();
	int		parseCgiHeader();
	void	respondStatusLine(int statusCode);
	void	respondHeader();

	virtual IoEventPoller::EventStatus	handleReadEventWork();
	virtual IoEventPoller::EventStatus	handleWriteEventWork();
	virtual IoEventPoller::EventStatus	handleErrorEventWork();
// member variables;
private:
	pid_t						m_pid;

	std::vector<std::string>	m_env;
	std::vector<char*>			m_envp;
	std::vector<std::string>	m_argvBase;
	std::vector<char*>			m_argv;
	std::string					m_cgiPath;

	RequestHandler*	m_requestHandler;

	int				m_requestContentFileFd;

	std::string		m_responseHeader;

	int				m_serverToCgi[2];
	int				m_cgiToServer[2];

	Buffer			m_fromCgiBuffer;
	Buffer*			m_toCgiBuffer;
	e_status		m_status;

	static std::vector<std::pair<int, int> >	s_cgiPipeList;

	static void		closePipeList();
};

#endif
