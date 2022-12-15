#ifndef ASocket_hpp
#define ASocket_hpp

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <vector>

using namespace std;

template<class T>
class ASocket
{

// type define
protected:
	typedef T sockType;

// deleted
private:
	ASocket(const ASocket& copy);
	ASocket& operator=(const ASocket& copy);

public:
// constructors & destructor
	ASocket() {};
	virtual ~ASocket() {};

// member functions
	virtual void	createSocket(const sockType& sockaddr) = 0;

protected:
// member variales
	struct sockaddr_in	m_socketAddr;
	socklen_t			m_socketAddrSize;
	int					m_socketFd;
};

#endif //ASocket_hpp
