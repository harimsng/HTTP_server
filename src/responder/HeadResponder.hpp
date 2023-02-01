#ifndef HEADRESPONDER_HPP
#define HEADRESPONDER_HPP

#include "responder/AResponder.hpp"

class	HeadResponder: public AResponder
{
public:
// constructors & destructor
	HeadResponder(RequestHandler& requestHandler);
	~HeadResponder();

// operators
	HeadResponder	&operator=(const HeadResponder& HeadResponder);

// member functions
	virtual void	respond();
};

#endif
