#include "PostResponder.hpp"

// constructors & destructor
PostResponder::PostResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}
PostResponder::~PostResponder()
{
}

// operators
PostResponder&
PostResponder::operator=(const PostResponder& postResponder)
{
	(void)postResponder;
	return *this;
}

void
PostResponder::respond()
{
	switch (m_methodStatus)
	{
		case AResponder::HEADER:
		case AResponder::BODY:
		case AResponder::DONE:
			;
	}
}

/*
 * event notified
 *
 * read
 * 		server
 * 				accept()
 * 		client
 *				requestHandler
 *				clientSocket ->   receiveRequest()
 *				parsed request -> createResponseHeader()
 *				method
 *				clientSocket ->	  method->respond()
				
 *										respond
 *
 * write
 */
