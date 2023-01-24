#ifndef HEADMETHOD_HPP
#define HEADMETHOD_HPP

#include "http/AMethod.hpp"
class	HeadMethod : public AMethod
{
public:
// constructors & destructor
	HeadMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~HeadMethod();

// operators
	HeadMethod	&operator=(const HeadMethod& headMethod);

// member functions
	virtual void	completeResponse();
};

#endif
