#ifndef PUTRESPONDER_HPP
#define PUTRESPONDER_HPP

#include "responder/AResponder.hpp"

class	PutResponder : public AResponder
{
public:
// constructors & destructor
	PutResponder(RequestHandler& requestHandler);
	~PutResponder();

// operators
	PutResponder	&operator=(const PutResponder& putResponder);

// member functions
	virtual void	respond();
};

#endif
