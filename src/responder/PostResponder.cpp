#include "PostResponder.hpp"

// constructors & destructor
PostResponder::PostResponder()
{
}

PostResponder::~PostResponder()
{
}

PostResponder::PostResponder(const PostResponder& postresponder)
{
	(void)postresponder;
}

// operators
PostResponder&
PostResponder::operator=(const PostResponder& postresponder)
{
	(void)postresponder;
	return *this;
}
