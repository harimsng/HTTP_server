#ifndef ARESPONDER_HPP
#define ARESPONDER_HPP

#include "http/RequestHandler.hpp"

#define RESPONDER_TRANSITION(state)\
	m_responseState = state; (this->*(s_transitionTable[state]))();

class	AResponder
{
private:
	AResponder();
	AResponder(const AResponder& method);

public:
	enum e_responseState {
		RES_HEADER = 0,
		RES_RECV_CONTENT,
		RES_RECV_CONTENT_DONE,
		RES_SEND_TO_CGI,
		RES_DONE
	};

// constructors & destructor
	AResponder(RequestHandler& requestHandler);
	virtual ~AResponder();

// operators
	AResponder	&operator=(const AResponder& aMethod);

// member functions
	void			respond();
	virtual	void	respondWork() = 0;
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

	e_responseState		m_responseState;

	int					m_dataSize;
	int					m_totalContentLentgh;

	int					m_fileFd;

	int			(AResponder::*m_recvContentFunc)();
	int			(AResponder::*m_sendContentFunc)();
	int			(AResponder::*m_procContentFunc)(size_t size);
};

#endif
