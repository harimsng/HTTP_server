#include "PostMethod.hpp"

// constructors & destructor
PostMethod::PostMethod()
{
}

PostMethod::~PostMethod()
{
}

PostMethod::PostMethod(const PostMethod& postMethod)
{
	(void)postMethod;
}

// operators
PostMethod&
PostMethod::operator=(const PostMethod& postMethod)
{
	(void)postMethod;
	return *this;
}
