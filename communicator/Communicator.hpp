#ifndef Communicator_hpp
#define Communicator_hpp

#include <unistd.h>
#include <cstddef>
#include <string>
#include <vector>
#include <map>

#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"

class Communicator
{
	friend class ClientSocket;

public:
	typedef Request::requestHeaderMap requestHeaderMap;

private:
// memver variales
	requestHeaderMap		m_requestHeader;
	std::string				m_methodType;
	std::string				m_uri;
	Response				m_response;
	Request					m_request;
	int						m_preBufferSize;

	Communicator(const Communicator& copy);
	Communicator& operator=(const Communicator& copy);
public:
// constructors & destructor
	Communicator();
	~Communicator();

// member functions
	int		readRequestHeader(std::string& requestMessageBuffer);
	int		readRequestBody(std::string& requestMessageBuffer);
	void	writeSocket(int fd);
};
#endif //Communicator_hpp
