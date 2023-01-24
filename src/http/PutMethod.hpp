#ifndef PUTMETHOD_HPP
#define PUTMETHOD_HPP

#include "http/AMethod.hpp"

class	PutMethod : public AMethod
{
public:
// constructors & destructor
	PutMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~PutMethod();

// operators
	PutMethod	&operator=(const PutMethod& putMethod);

// member functions
	virtual void	completeResponse();
};

#endif
