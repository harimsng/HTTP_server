#ifndef ClientSocket_hpp
#define ClientSocket_hpp

#include <fcntl.h>

#include "socket/ASocket.hpp"
#include "socket/ServerSocket.hpp"
#include "communicator/Request.hpp"
#include "communicator/Response.hpp"
// #include "Communicator.hpp"

class	Server;

class ClientSocket : public ASocket<ServerSocket>
{
	friend class Server;

	std::string		m_requestBuffer;
	std::string		m_responseBuffer;

	Request			m_request;
	Response		m_response;

public:
// constructors & destructor
	ClientSocket();
	ClientSocket(const ClientSocket& copy);
	~ClientSocket();

// operator
	ClientSocket& operator=(const ClientSocket& copy);
// member functions
	virtual void	createSocket(const sockType& initClass);
	virtual void	readSocket(int messagesize);
	void			writeSocket();
	void			checkUri(Server& server);

};

#endif //ClientSocket_hpp
