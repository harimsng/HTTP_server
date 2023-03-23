#ifndef PUTRESPONDER_HPP
#define PUTRESPONDER_HPP

#include "responder/AResponder.hpp"

class	PutResponder : public AResponder
{
public:
	PutResponder(RequestHandler& requestHandler);
	~PutResponder();

	PutResponder	&operator=(const PutResponder& putResponder);

	virtual void	respondWork();
};

#endif
