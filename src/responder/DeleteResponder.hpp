#ifndef DELETERESPONDER_HPP
#define DELETERESPONDER_HPP

#include "responder/AResponder.hpp"

class	DeleteResponder : public AResponder
{
public:
	DeleteResponder(RequestHandler& requestHandler);
	~DeleteResponder();

	DeleteResponder	&operator=(const DeleteResponder& deleteMethod);

	void			deleteFile(const std::string& filePath, std::string& readBody);
	virtual void	respondWork();
};

#endif
