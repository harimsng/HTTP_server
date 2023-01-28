#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include "http/RequestHandler.hpp"

class	AResponder
{
// deleted
	AResponder(const AResponder& aresponder);
	AResponder	&operator=(const AResponder& aresponder);

public:
// constructors & destructor
	AResponder(const RequestHandler& requestHandler);
	~AResponder();

// member functions
	void	bufferResponsedHeader();
	void	bufferResponseContent();

// member variables
private:
	const RequestHandler* const	m_requestHandler;
};

#endif
