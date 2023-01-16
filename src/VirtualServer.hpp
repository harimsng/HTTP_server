#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include "Webserv.hpp"
#include "Location.hpp"
#include "event/Client.hpp"
#include "socket_/Socket.hpp"

using namespace std;

class	VirtualServer
{
	typedef	std::map<std::string, Location>	LocationTable;
public:
// constructors & destructor
	VirtualServer();
	~VirtualServer();
	VirtualServer(VirtualServer const& server);
	VirtualServer	&operator=(VirtualServer const& server);

// member functions

private:
	void	setToDefault();

// member variales - config
	std::string							m_index;
	bool								m_autoindex;
	std::vector<std::string>			m_serverNames;
	std::string							m_errorCode;
	std::string							m_root;
	std::string							m_errorPath;
	std::string							m_errorPages;
	sockaddr_in							m_listen;
	int32_t								m_clientMaxBodySize;
	int32_t								m_uriBufferSize;
	std::map<std::string, Location>		m_locationTable;
	uint64_t	m_addrKey;

// friends
	friend class			ConfigParser;
	friend class			ServerParser;
	friend std::string 		Response::getResourceLocation(const std::string& host);
	friend std::ostream&	operator<<(std::ostream& os, const VirtualServer& server);
};

#endif
