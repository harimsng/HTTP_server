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

	char	front() const;
	char	back() const;

	void	pop_back();
};

class	ReceiveBuffer: public Buffer
{
public:
	std::string::size_type	receive(int fd);
};

class	SendBuffer: public Buffer
{
public:
	std::string::size_type	send(int fd);

private:
	std::string::size_type	m_writePos;
};

#endif
