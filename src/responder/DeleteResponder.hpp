#ifndef DELETERESPONDER_HPP
#define DELETERESPONDER_HPP

#include "responder/AResponder.hpp"

class	DeleteResponder : public AResponder
{
public:
// constructors & destructor
	DeleteResponder(RequestHandler& requestHandler);
	~DeleteResponder();

// operators
	DeleteResponder	&operator=(const DeleteResponder& deleteMethod);

	void			deleteFile(const std::string& filePath, std::string& readBody);
	virtual void	respond();
// member functions
};

#endif
