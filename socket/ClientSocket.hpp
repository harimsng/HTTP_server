#ifndef ClientSocket_hpp
#define ClientSocket_hpp

#include <fcntl.h>

#include "ASocket.hpp"
#include "ServerSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
// #include "Communicator.hpp"

class ClientSocket : public ASocket<ServerSocket>
{
	friend class Server;

	std::string		m_requestBuffer;

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
	virtual void	createSocket(const initType& initClass);
	void			readSocket(int messagesize, Server& server);
	void			writeSocket();

};
#endif //ClientSocket_hpp
