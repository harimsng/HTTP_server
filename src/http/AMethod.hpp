#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include "io/Buffer.hpp"

struct	Request;

class	AMethod
{
private:
	AMethod();
	AMethod(const AMethod& method);
public:
// constructors & destructor
	AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~AMethod();

// operators
	AMethod	&operator=(const AMethod& aMethod);

// member functions
	virtual void	completeResponse() = 0;
};

#endif
