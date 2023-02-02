#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include <fstream>

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

	std::string		parseChunkSize();
	void			openFile();
	void			writeFile(int writeSize);
	void			readFile(std::string& readBody);
	void			readRequestBody();
	int				normalReadBody();
	int				chunkedReadBody();
	bool			checkFileExists(const std::string& filePath);
	bool			checkDirExists(const std::string& filePath);

protected:
	RequestHandler&		m_requestHandler;
	Request&			m_request;
	SendBuffer&			m_sendBuffer;
	ReceiveBuffer&		m_recvBuffer;
	e_methodStatus		m_methodStatus;
	int					m_chunkedSize;
	std::fstream		m_file;
};

#endif
