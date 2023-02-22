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
	enum	e_state
	{
		CGI_HEADER = 1,
		CGI_CONTENT
	};
// deleted
	Cgi	&operator=(Cgi const& cgi);
	Cgi(Cgi const& cgi);

public:
// constructors & destructor
	Cgi(int cgiToServer[2], int serverToCgi[2], RequestHandler& requestHandler, Buffer& toCgiBuffer);
	Cgi(int cgiToServer[2], RequestHandler& requestHandler);
	~Cgi();

// member functions
	virtual IoEventPoller::EventStatus	handleReadEventWork();
	virtual IoEventPoller::EventStatus	handleWriteEventWork();
	virtual IoEventPoller::EventStatus	handleErrorEventWork();

private:
	void	initialize(const Request& request);
	void	executeCgi(int cgiToServer[2], int serverToCgi[2]);
	void	executeCgi(int cgiToServer[2]);

	int		receiveCgiResponse();
	int		sendCgiRequest();
	int		parseCgiHeader();
	void	respondStatusLine(int statusCode);
	void	respondHeader();

// member variables;
private:
	int			m_serverToCgi;
	pid_t		m_pid;

	std::vector<std::string>	m_envpBase;
	std::vector<std::string>	m_argvBase;
	std::vector<char*>			m_envp;
	std::vector<char*>			m_argv;
	std::string					m_cgiPath;

	RequestHandler*	m_requestHandler;

	std::string		m_responseHeader;

	Buffer		m_fromCgiBuffer;
	Buffer*		m_toCgiBuffer;
	e_state		m_state;

	static std::vector<int>	s_cgiPipeList;

	static void		closePipeList();
};

#endif
