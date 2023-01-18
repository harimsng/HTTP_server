#include "DeleteMethod.hpp"

// constructors & destructor
DeleteMethod::DeleteMethod()
{
}

DeleteMethod::~DeleteMethod()
{
}

DeleteMethod::DeleteMethod(const DeleteMethod& deleteMethod)
{
	(void)deleteMethod;
}

// operators
DeleteMethod&
DeleteMethod::operator=(const DeleteMethod& deleteMethod)
{
	(void)deleteMethod;
	return *this;
}
