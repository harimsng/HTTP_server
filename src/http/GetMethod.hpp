#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

#include "http/AMethod.hpp"

class	GetMethod : public AMethod
{
public:
// constructors & destructor
	GetMethod(RequestHandler& requestHandler);
	~GetMethod();

// operators
	GetMethod	&operator=(const GetMethod& getMethod);

	virtual void	completeResponse();
// member functions
};

#endif
