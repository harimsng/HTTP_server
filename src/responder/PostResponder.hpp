#ifndef POSTRESPONDER_HPP
#define POSTRESPONDER_HPP

#include "responder/AResponder.hpp"

class	PostResponder : public AResponder
{
public:
// constructors & destructor
	PostResponder(RequestHandler& requestHandler);
	~PostResponder();

// operators
	PostResponder	&operator=(const PostResponder& postResponder);

// member functions
<<<<<<< HEAD
	virtual void	respond();
=======
	virtual void	respondWork();
	// void			constructCgi(std::string& readBody);
>>>>>>> e5cb41c3c22b0c5f7c9eaf4de50428846f569075
	void			constructCgi();

private:
};

#endif
