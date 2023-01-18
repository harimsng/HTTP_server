#include "GetMethod.hpp"

// constructors & destructor
GetMethod::GetMethod()
{
}

GetMethod::~GetMethod()
{
}

GetMethod::GetMethod(const GetMethod& getMethod)
{
	(void)getMethod;
}

// operators
GetMethod&
GetMethod::operator=(const GetMethod& getMethod)
{
	(void)getMethod;
	return *this;
}
