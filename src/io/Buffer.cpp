#include <stdexcept>
#include <unistd.h>

#include "Types.hpp"
#include "Buffer.hpp"

using namespace std;

#define BUFFER_SIZE (8192 << 1)

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
	const t_uint64	residue = size();
	t_int64			count;

	if (residue == BUFFER_SIZE - 1)
		return -2;
	resize(BUFFER_SIZE, 0);
	count = ::read(fd, &(*this)[0] + residue,
			size() - residue - 1);
	if (count <= 0)
		return count;

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

#include "Logger.hpp"

std::string::size_type
Buffer::send(int fd)
{
	t_int64		count = 0;
	t_uint64	writeSize;

	writeSize = size() - m_writePos;
	// NOTE: this if stetement and BUFFER_SIZE are important part.
	// if Cgi has been fed with large size of write, write blocks.
	// if BUFFER_SIZE not large enough (BUFFER_SIZE < (8192 << 3)), final stage of the test would block.
	// if (size() - m_writePos > BUFFER_SIZE)
	//	writeSize = BUFFER_SIZE;

	count = ::write(fd, data() + m_writePos, writeSize);
//	LOG(DEBUG, "write writesize = %ld, count = %ld", writeSize, count);
	if (count <= 0)
		return count;
	
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
