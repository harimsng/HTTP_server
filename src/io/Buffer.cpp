#include <stdexcept>
#include <unistd.h>

#include "Types.hpp"
#include "Buffer.hpp"

using namespace std;

#define BUFFER_SIZE (200000)

// deleted
Buffer::Buffer(const Buffer& buffer)
:	string()
{
	if (this != &buffer)
		*this = buffer;
}

Buffer&
Buffer::operator=(const Buffer& buffer)
{
	(void)buffer;
	return *this;
}
// constructors & destructor
Buffer::Buffer()
:	std::string(BUFFER_SIZE, 0),
	m_writePos(0),
	// NOTE: initialize enum member
	m_status(BUF_GOOD)
{
	resize(0);
}

Buffer::~Buffer()
{
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
Buffer::read(int fd)
{
	if (size() == BUFFER_SIZE)
		return 0;
	clear();
	return ::read(fd, &(*this)[0], BUFFER_SIZE - size());
}

std::string::size_type
Buffer::receive(int fd)
{
	const int		residue = size();
	long int	count = 0;

	resize(BUFFER_SIZE, 0);
	count = ::read(fd, &(*this)[0] + residue,
			size() - residue - 1);
	if (count == -1)
		throw std::runtime_error("read() fail in Buffer::receive()");

	resize(residue + count, 0);
	return count;
}

std::string::size_type
Buffer::receive()
{
	return receive(m_fd);
}

std::string::size_type
Buffer::write(int fd)
{
	int	written;

	if (size() == 0)
		return 0;

	written = ::write(fd, data(), size());
	clear();
	return written;
}

std::string::size_type
Buffer::mysend(int fd)
{
	long int	count = 0;
	t_uint64	writeSize;

	if (size() == 0)
		return 0;

	writeSize = size() - m_writePos;
	// heuristic solution. consider to set the fd non-blocking
	if (size() - m_writePos > 8192)
		writeSize = 8192;

	count = ::write(fd, data() + m_writePos, writeSize);
	if (count == -1)
		throw std::runtime_error("write() fail in Buffer::send()");

	m_writePos += count;
	if (m_writePos == size())
	{
		m_writePos = 0;
		clear();
	}
	return count;
}

std::string::size_type
Buffer::send(int fd)
{
	long int	count = 0;
	t_uint64	writeSize;

	if (size() == 0)
		return 0;

	writeSize = size() - m_writePos;
	// heuristic solution. consider to set the fd non-blocking
	if (size() - m_writePos > BUFFER_SIZE)
		writeSize = BUFFER_SIZE;

	count = ::write(fd, data() + m_writePos, writeSize);
	if (count == -1)
		throw std::runtime_error("write() fail in Buffer::send()");

	m_writePos += count;
	if (m_writePos == size())
	{
		m_writePos = 0;
		clear();
	}
	return count;
}

std::string::size_type
Buffer::send()
{
	return this->send(m_fd);
}

Buffer::e_status
Buffer::status()
{
	return m_status;
}

void
Buffer::status(e_status stat)
{
	m_status = stat;
}

void
Buffer::setFd(int fd)
{
	m_fd = fd;
}
