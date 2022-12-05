#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/select.h>

#include <cstddef>

class	IResponse;
class	IRequest;

class	Connection
{
// static members
	static std::size_t const	s_bufferSize = 1024;
	static fd_set*				s_writeSetPtr;

// discarded (hidden instead of deletion because C++98 doesn't have function delete feature)
	Connection	&operator=(Connection const& connection);

public:
// constructors & destructor
	Connection(int socket);
	~Connection();
	Connection(Connection const& connection);

// operators

// member functions
	int		initiateWrite();
	int		writeData();
	int		readData();

private:
// member variables
	int const		m_socket;
	IRequest*		m_request;
	IResponse*		m_response;
//	unsigned char	m_readBuffer[s_bufferSize];
//	unsigned char	m_writeBuffer[s_bufferSize];
	std::size_t		m_size;
	std::size_t		m_pos;
	
};

#endif
