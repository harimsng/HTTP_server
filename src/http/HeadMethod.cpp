#include "HeadMethod.hpp"

// constructors & destructor
HeadMethod::HeadMethod()
{
}

HeadMethod::~HeadMethod()
{
}

HeadMethod::HeadMethod(const HeadMethod& headMethod)
{
	(void)headMethod;
}

// operators
HeadMethod&
HeadMethod::operator=(const HeadMethod& headMethod)
{
	(void)headMethod;
	return *this;
}
