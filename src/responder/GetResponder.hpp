#ifndef GETRESPONDER_HPP
#define GETRESPONDER_HPP

#include "responder/AResponder.hpp"

class	GetResponder : public AResponder
{
public:
	GetResponder(RequestHandler& requestHandler);
	~GetResponder();

	GetResponder	&operator=(const GetResponder& getMethod);

	virtual void	respondWork();

	void			constructCgi();
	bool			isAutoIndex();
};

#endif
