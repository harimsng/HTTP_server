#ifndef POSTRESPONDER_HPP
#define POSTRESPONDER_HPP

#include "responder/AResponder.hpp"

class	PostResponder : public AResponder
{
	typedef void	(PostResponder::*t_transition)();

public:
// constructors & destructor
	PostResponder(RequestHandler& requestHandler);
	~PostResponder();

// operators
	PostResponder	&operator=(const PostResponder& postResponder);

// member functions
	virtual void	respondWork();

	void	header();
	void	recvContent();
	void	recvContentDone();
	void	sendToCgi();
	void	done();

	void	constructCgi();

private:
	static const t_transition	s_transitionTable[5];
};

#endif
