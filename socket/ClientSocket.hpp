#ifndef ClientSocket_hpp
#define ClientSocket_hpp

#include <fcntl.h>

#include "ASocket.hpp"
#include "ServerSocket.hpp"

class ClientSocket : public ASocket<ServerSocket>
{
	friend class Server;

public:
// constructors & destructor
	ClientSocket();
	ClientSocket(const ClientSocket& copy);
	~ClientSocket();

// operator
	ClientSocket& operator=(const ClientSocket& copy);
// member functions
	virtual void	createSocket(const initType& initClass);
};
#endif //ClientSocket_hpp
