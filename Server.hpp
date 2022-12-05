#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>

#include "Location.hpp"

class	Server
{
	friend class	ServerParser;

	Server	&operator=(Server const& server);

public:
// constructors & destructor
	Server();
	~Server();
	Server(Server const& server) {(void)server;};

// member functions

// member variales
	std::string		m_index;
	std::string		m_serverName;
	std::string		m_errorCode;
	std::string		m_root;
	std::string		m_errorPath;
	int32_t			m_listen;
	int32_t			m_clientMaxBodySize;
	int32_t			m_uriBufferSize;

	std::vector<Location>	m_locationList;
};

#endif
