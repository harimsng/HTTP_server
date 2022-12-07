#ifndef ServerSocket_hpp
#define ServerSocket_hpp

#include "ASocket.hpp"

class Server;

#define BACKLOG 256

class ServerSocket : public ASocket<Server>
{
	friend class ClientSocket;
	friend class Server;

// deleted
	ServerSocket(const ServerSocket& copy);
	ServerSocket& operator=(const ServerSocket& copy);


public:
// constructors & destructor
	ServerSocket();
	~ServerSocket();

// member functions
	virtual void	createSocket(const initType& initClass);

private:
	void			setSocketFd(int domain, int type, int protocol);
	void			initAddr(sa_family_t sin_family, in_port_t sin_port,
							 in_addr_t s_addr);
	void			bindSocket();
	void			listenSocket();

};
#endif //ServerSocket_hpp
