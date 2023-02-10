#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>

class	Buffer: public std::string
{
public:
	enum	e_status
	{
		BUF_GOOD = 0x1,
		BUF_EOF = 0x2
	};

// constructors & destructor
	Buffer();
	~Buffer();
	Buffer(const Buffer& buffer);

// operators
	Buffer	&operator=(const Buffer& buffer);

// member functions
	std::string::size_type	read(int fd);
	std::string::size_type	receive(int fd);
	std::string::size_type	receive();

	std::string::size_type	write(int fd);
	std::string::size_type	send(int fd);
	std::string::size_type	send();
	void					setFd(int fd);

	e_status	status();
	void		status(e_status stat);

	char	front() const;
	char	back() const;
	void	pop_back();

private:
//	std::string::size_type	m_writePos;
	int						m_fd;
	e_status				m_status;
};

class	ReceiveBuffer: public Buffer
{
private:
	std::string::size_type	send(int fd);
	std::string::size_type	send();
};

class	SendBuffer: public Buffer
{
private:
	std::string::size_type	receive(int fd);
	std::string::size_type	receive();
};

#endif
