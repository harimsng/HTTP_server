#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include "io/Buffer.hpp"

struct	Request;

class	AMethod
{
public:
// constructors & destructor
	AMethod();
	~AMethod();
	AMethod(const AMethod& method);
	AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);

// operators
	AMethod	&operator=(const AMethod& aMethod);

// member functions
	void	completeResponse();
};

#endif
