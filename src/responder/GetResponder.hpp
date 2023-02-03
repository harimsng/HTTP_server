#ifndef GETRESPONDER_HPP
#define GETRESPONDER_HPP

#include "responder/AResponder.hpp"

class	GetResponder : public AResponder
{
public:
// constructors & destructor
	GetResponder(RequestHandler& requestHandler);
	~GetResponder();

// operators
	GetResponder	&operator=(const GetResponder& getMethod);

	virtual void	respond();

	void			constructCgi(std::string& readBody);
// member functions
};

#endif
