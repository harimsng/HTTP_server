#include <stdexcept>
#include <unistd.h>

#include "Buffer.hpp"

#define BUFFER_SIZE (8192)

// constructors & destructor
Buffer::Buffer()
:	std::string(BUFFER_SIZE, 0)
{
	resize(0);
}

Buffer::~Buffer()
{
}

Buffer::Buffer(const Buffer& buffer)
{
	if (this != &buffer)
		*this = buffer;
}

// operators
Buffer&
Buffer::operator=(const Buffer& buffer)
{
	(void)buffer;
	return *this;
}

char
Buffer::front() const
{
	return *begin();
}

char
Buffer::back() const
{
	return *(end() - 1);
}

void
Buffer::pop_back()
{
	erase(end() - 1);
}

std::string::size_type
Buffer::receive(int fd)
{
	const int		residue = size();
	long long int	count = 0;

	resize(BUFFER_SIZE, 0);
	count = ::read(fd, &(*this)[0] + residue,
			size() - residue - 1);
	if (count < -1)
		throw std::runtime_error("read() fail in Buffer::receive()");

	resize(residue + count, 0);
	return count;
}

std::string::size_type
Buffer::send(int fd)
{
	long long int	count = 0;

	if (size() == 0)
		return 0;
	count = ::write(fd, data() + m_writePos, size() - m_writePos);
	if (count == -1)
		throw std::runtime_error("read() fail in Buffer::receive()");

	m_writePos += count;
	if (m_writePos == size())
	{
		m_writePos = 0;
		clear();
	}
	return count;
}

std::string::size_type
Buffer::receive()
{
	const int		residue = size();
	long long int	count = 0;

	resize(BUFFER_SIZE, 0);
	count = ::read(m_fd, &(*this)[0] + residue,
			size() - residue - 1);
	if (count < -1)
		throw std::runtime_error("read() fail in Buffer::receive()");

	resize(residue + count, 0);
	return count;
}

std::string::size_type
Buffer::send()
{
	long long int	count = 0;

	if (size() == 0)
		return 0;
	count = ::write(m_fd, data() + m_writePos, size() - m_writePos);
	if (count == -1)
		throw std::runtime_error("read() fail in Buffer::receive()");

	m_writePos += count;
	if (m_writePos == size())
	{
		m_writePos = 0;
		clear();
	}
	return count;
}

void
Buffer::setFd(int fd)
{
	m_fd = fd;
}
