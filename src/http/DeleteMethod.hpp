#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include "http/AMethod.hpp"

class	DeleteMethod : public AMethod
{
public:
// constructors & destructor
	DeleteMethod(RequestHandler& requestHandler);
	~DeleteMethod();

// operators
	DeleteMethod	&operator=(const DeleteMethod& deleteMethod);

	virtual void	completeResponse();
// member functions
	bool			checkMethodLimit(const std::vector<std::string>& limitExcept) const;
};

#endif
