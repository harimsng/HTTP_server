#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>

#include <vector>
#include <string>

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>

#include "Location.hpp"

#define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})

class	Server
{
	friend class	ServerParser;
	friend class	ConfigParser;

public:
// constructors & destructor
	Server();
	~Server();
	Server(Server const& server);
	Server	&operator=(Server const& server);

// member functions


private:
	void	setToDefault();

// member variales
	std::string		m_index;
	std::string		m_serverNames;
	std::string		m_errorCode;
	std::string		m_root;
	std::string		m_errorPages;
	sockaddr_in		m_listen;
	int32_t		m_clientMaxBodySize;
	int32_t		m_uriBufferSize; // it correspond to what option in nginx config?

	std::vector<Location>	m_locationList;

	friend std::ostream&	operator<<(std::ostream& os, const Server& server);
};

#endif
