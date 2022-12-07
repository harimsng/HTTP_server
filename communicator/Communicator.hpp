#ifndef Communicator_hpp
#define Communicator_hpp

#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "Request.hpp"
#include "Response.hpp"

using namespace std;

class Communicator
{
public:
	typedef Request::requestHeaderMap requestHeaderMap;

private:
// memver variales
	requestHeaderMap	m_requestHeader;
	Request				m_request;
	string				m_stringBuffer;
	int					m_socketFd;


public:
// constructors & destructor
	Communicator(int socketFd);
	Communicator(const Communicator& copy);
	~Communicator();

// operator
	Communicator& operator=(const Communicator& copy);

// member functions
	void	readSocket(int messageSize);
	void	writeSocket();
};
#endif //Communicator_hpp
