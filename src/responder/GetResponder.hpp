#ifndef GETRESPONDER_HPP
#define GETRESPONDER_HPP

#include "responder/AResponder.hpp"

class	GetResponder : public AResponder
{
	typedef void	(GetResponder::*t_transition)();

public:
// constructors & destructor
	GetResponder(RequestHandler& requestHandler);
	~GetResponder();

// operators
	GetResponder	&operator=(const GetResponder& getMethod);

// member functions
	virtual void	respondWork();

	void	header();
	void	recvContent();
	void	done();

	void	constructCgi();
	bool	isAutoIndex();

private:
	static const t_transition	s_transitionTable[5];
};

#endif
