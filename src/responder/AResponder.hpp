#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include "http/RequestHandler.hpp"

class	AResponder
{
private:
	AResponder();
	AResponder(const AResponder& method);

public:
	enum e_methodStatus{
		HEADER = 0,
		BODY,
		DONE
	};
public:
// constructors & destructor
	AResponder(RequestHandler& requestHandler);
	virtual ~AResponder();

// operators
	AResponder	&operator=(const AResponder& aMethod);

// member functions
	virtual void	respond() = 0;
	void			respondHeader();
	void			endResponse();

	void			readFile(std::string& readBody);
	bool			checkFileExists(const std::string& filePath);
	bool			checkDirExists(const std::string& filePath);

protected:
	RequestHandler&		m_requestHandler;
	Request&			m_request;
	SendBuffer&			m_sendBuffer;
	ReceiveBuffer&		m_recvBuffer;
	e_methodStatus		m_methodStatus;
};

#endif
