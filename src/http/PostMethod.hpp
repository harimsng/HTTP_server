#ifndef POSTMETHOD_HPP
#define POSTMETHOD_HPP

#include "http/AMethod.hpp"

class	PostMethod : public AMethod
{
public:
// constructors & destructor
	PostMethod(RequestHandler& requestHandler);
	~PostMethod();

// operators
	PostMethod	&operator=(const PostMethod& postMethod);

// member functions
	virtual void	completeResponse();
};

#endif
