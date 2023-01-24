#ifndef POSTMETHOD_HPP
#define POSTMETHOD_HPP

#include "http/AMethod.hpp"
class	PostMethod : public AMethod
{
public:
// constructors & destructor
	PostMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~PostMethod();

// operators
	PostMethod	&operator=(const PostMethod& postMethod);

// member functions
	virtual void	completeResponse();
};

#endif
