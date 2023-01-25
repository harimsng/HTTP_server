#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>

class	Buffer: public std::string
{
public:
// constructors & destructor
	Buffer();
	~Buffer();
	Buffer(const Buffer& buffer);

// operators
	Buffer	&operator=(const Buffer& buffer);

// member functions
	const std::string&		getBuffer() const;
	std::string::size_type	receive(int fd);
	std::string::size_type	send(int fd);
	std::string::size_type	receive();
	std::string::size_type	send();
	void					setFd(int fd);

	char	front() const;
	char	back() const;
	void	pop_back();

private:
	std::string::size_type	m_writePos;
	int						m_fd;
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
