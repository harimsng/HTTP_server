#ifndef PUTRESPONDER_HPP
#define PUTRESPONDER_HPP

#include "responder/AResponder.hpp"

class	PutResponder : public AResponder
{
	typedef	void	(PutResponder::*t_transition)();

public:
// constructors & destructor
	PutResponder(RequestHandler& requestHandler);
	~PutResponder();

// operators
	PutResponder	&operator=(const PutResponder& putResponder);

// member functions
	virtual void	respondWork();

	void	header();
	void	recvContent();
	void	recvContentDone();
	void	done();

	static const t_transition	s_transitionTable[5];
};

#endif
