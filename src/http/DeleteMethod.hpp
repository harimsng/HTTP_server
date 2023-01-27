#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include "http/AMethod.hpp"

class	DeleteMethod : public AMethod
{
public:
// constructors & destructor
	DeleteMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~DeleteMethod();

// operators
	DeleteMethod	&operator=(const DeleteMethod& deleteMethod);

	virtual void	completeResponse();
// member functions
	bool			checkMethodLimit(const vector<string>& limitExcept) const;
};

#endif
