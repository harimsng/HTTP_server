#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include "http/RequestHandler.hpp"
#include "event/Cgi.hpp"

class Cgi;

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
	void			respondStatusLine(int statusCode);
	void			respondBody(const std::string& readBody);

	int				sendContentNormal();
	int				sendContentChunked();

	std::string		parseChunkSize();

	void			openFile(const std::string& path);
	void			openFile();
	int				writeToFile(size_t writeSize);
	int				writeToBuffer(size_t writeSize);
	void			readFile(std::string& readBody);

	int				receiveContentNormal();
	int				receiveContentChunked();

	bool			checkFileExists(const std::string& filePath);
	bool			checkDirExists(const std::string& filePath);

	std::string		getErrorPage(std::string& readBody);

	RequestHandler&		m_requestHandler;
	Request&			m_request;
	Buffer&				m_sendBuffer;
	Buffer&				m_recvBuffer;
	Buffer				m_buffer;
	Cgi*				m_cgi;

	e_responseStatus	m_responseStatus;
	int					m_dataSize;
	int					m_totalContentLentgh;
	int					m_fileFd;
	int					m_cgiWriteEnd;
	int					m_cgiReadEnd;
	Cgi*				m_cgi;

	int					m_serverToCgi;
	int			m_totalrecvBufferSize;
	int			(AResponder::*m_recvContentFunc)();
	int			(AResponder::*m_sendContentFunc)();
	int			(AResponder::*m_procContentFunc)(size_t size);
};

#endif
