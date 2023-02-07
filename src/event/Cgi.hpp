#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>
#include <sys/stat.h>
#include "event/EventObject.hpp"

class	RequestHandler;
struct	Request;

class	Cgi: public EventObject
{
// deleted
	Cgi	&operator=(Cgi const& cgi);
	Cgi(Cgi const& cgi);

public:
// constructors & destructor
	Cgi(int fileFd, int writeEnd, RequestHandler& requestHandler);
	Cgi(int* writeEnd, int* readEnd, RequestHandler& requestHandler);
	~Cgi();

// member functions
	void	initEnv(const Request& request);
	void	executeCgi(int pipe[2], std::string& readBody, const Request &request);
#ifdef TEST
	void	executeCgi(int pipe[2]);
#endif
	int		receiveCgiResponse();

	IoEventPoller::EventStatus	handleEventWork();

// member variables;
private:
		int							m_pid;

		std::vector<std::string>	m_env;
		std::vector<char*>			m_envp;
		std::vector<std::string>	m_argvBase;
		std::vector<char*>			m_argv;
		std::string					m_cgiPath;


		// int							m_fromCgiToServer[2];
		// int							m_fromServerToCgi[2];
		// pid_t						m_pid;
		bool						m_bodyFlag;
		RequestHandler*				m_requestHandler;

		int							m_requestContentFileFd;



		int*							m_readEnd;
		int*							m_writeEnd;
};

#endif
