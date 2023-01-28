#ifndef PUTMETHOD_HPP
#define PUTMETHOD_HPP

#include "http/AMethod.hpp"

class	PutMethod : public AMethod
{
public:
// constructors & destructor
	PutMethod(RequestHandler& requestHandler);
	~PutMethod();

// operators
	PutMethod	&operator=(const PutMethod& putMethod);

// member functions
	virtual void	completeResponse();
};

#endif
