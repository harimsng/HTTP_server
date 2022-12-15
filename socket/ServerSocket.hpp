#ifndef ServerSocket_hpp
#define ServerSocket_hpp

#include "socket/ASocket.hpp"

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
	virtual void	createSocket(const sockaddr_in& sockaddrInet);

private:
	void			setSocketFd(int domain, int type, int protocol);
	void			initAddr(const sockType& sockaddrInet);
	void			bindSocket();
	void			listenSocket();
};
#endif //ServerSocket_hpp
