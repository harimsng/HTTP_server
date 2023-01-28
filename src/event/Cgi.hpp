#ifndef CGI_HPP
#define CGI_HPP

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include "http/RequestHandler.hpp"

#define READ 0
#define WRITE 1
#define BUFFER_SIZE 65535

class	Cgi
{
// deleted
	Cgi	&operator=(Cgi const& cgi);

public:
// constructors & destructor
	Cgi();
	~Cgi();
	Cgi(Cgi const& cgi);

// member functions
	void	initCgi(const Request &request);

	template <typename IoEventPoller>
	typename IoEventPoller::EventStatus	handleEvent(const typename IoEventPoller::EventData& event);

// member variables;
private:
		std::vector<std::string>	m_env;
		std::vector<char*>			m_envChar;
		std::string					m_cgiPath;
		std::string 				m_path;
		std::string					m_script;
		std::string					m_query;

		int							m_fromCgiToServer[2];
		int							m_fromServerToCgi[2];
		pid_t						m_pid;
		bool						m_bodyFlag;
};

template <typename IoEventPoller>
typename IoEventPoller::EventStatus	Cgi::handleEvent(const typename IoEventPoller::EventData& event)
{
	typename IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			return IoEventPoller::STAT_ERROR;
		case IoEventPoller::WRITE:
			return IoEventPoller::STAT_ERROR;
		default:
			throw std::runtime_error("not handled event filter in Cgi::handleEvent()");
	}
	return IoEventPoller::STAT_ERROR;
}

#endif
