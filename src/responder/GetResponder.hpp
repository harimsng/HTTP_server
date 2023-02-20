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

	virtual void	respondWork();

	void			constructCgi(std::string& readBody);
	bool			isAutoIndex();
// member functions
};

#endif
