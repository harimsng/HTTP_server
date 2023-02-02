#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include "http/RequestHandler.hpp"

class	AResponder
{
private:
	AResponder();
	AResponder(const AResponder& method);

public:
	enum e_responseStatus{
		RES_HEADER = 0,
		RES_CONTENT,
		RES_CONTENT_FINISHED,
		RES_SEND_CGI,
		RES_RECV_CGI,
		RES_DONE
	};

// constructors & destructor
	AResponder(RequestHandler& requestHandler);
	virtual ~AResponder();

// operators
	AResponder	&operator=(const AResponder& aMethod);

// member functions
	virtual void	respond() = 0;
	void			respondHeader();
	void			endResponse();

protected:
	std::string		parseChunkSize();

	void			openFile(const std::string& path);
	void			openFile();
	void			writeFile(int writeSize);
	void			readFile(std::string& readBody);

	void			readRequestBody();
	int				normalReadBody();
	int				chunkedReadBody();

	bool			checkFileExists(const std::string& filePath);
	bool			checkDirExists(const std::string& filePath);

	void			constructCgi();

	RequestHandler&		m_requestHandler;
	Request&			m_request;
	Buffer&				m_sendBuffer;
	Buffer&				m_recvBuffer;
	e_responseStatus	m_responseStatus;
	int					m_dataSize;
	int					m_fileFd;
	int					m_cgiReadEnd;
};

#endif
