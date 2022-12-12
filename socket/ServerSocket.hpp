#ifndef ServerSocket_hpp
#define ServerSocket_hpp

#include "ASocket.hpp"

class Server;

#define BACKLOG 256

class ServerSocket : public ASocket<sockaddr_in>
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
	void			initAddr(const initType& initClass);
	void			bindSocket();
	void			listenSocket();

};
#endif //ServerSocket_hpp
