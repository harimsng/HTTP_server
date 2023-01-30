#ifndef VIRTUALSERVER_HPP
#define VIRTUALSERVER_HPP

#include <vector>
#include <map>

#include "Webserv.hpp"
#include "http/AMethod.hpp"
#include "http/RequestHandler.hpp"

class	VirtualServer
{
	friend class	FindLocation;
	friend class	AMethod;

	typedef	std::map<std::string, Location>	LocationTable;

public:
// constructors & destructor
	VirtualServer();
	~VirtualServer();
	// VirtualServer(VirtualServer const& server) = default;
	// VirtualServer	&operator=(VirtualServer const& server) = default;

// member functions
private:
	void	setToDefault();

// member variales - config
	bool								m_autoindex;
	int32_t								m_clientMaxBodySize;
	std::string							m_root;
	std::vector<std::string>			m_index;
	std::vector<std::string>			m_serverNames;
	std::map<int, std::string>			m_errorPageTable;
	std::map<std::string, Location>		m_locationTable;

	uint64_t							m_addrKey;
	sockaddr_in							m_listen;
// friends
	friend class			ConfigParser;
	friend class			ServerParser;
	friend void				RequestHandler::createResponseHeader();
	friend std::ostream&	operator<<(std::ostream& os, const VirtualServer& server);
};

#endif
