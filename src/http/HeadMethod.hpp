#ifndef HEADMETHOD_HPP
#define HEADMETHOD_HPP

#include "http/AMethod.hpp"

class	HeadMethod : public AMethod
{
public:
// constructors & destructor
	HeadMethod(RequestHandler& requestHandler);
	~HeadMethod();

// operators
	HeadMethod	&operator=(const HeadMethod& headMethod);

// member functions
	virtual void	completeResponse();
};

#endif
