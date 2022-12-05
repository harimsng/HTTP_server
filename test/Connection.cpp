#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>

#include <string>

#include "Connection.hpp"

// discarded
Connection&	Connection::operator=(Connection const& connection)
{
	return *this;
}

// constructors & destructor
Connection::Connection(int socket)
:	m_socket(socket)
{
}

Connection::~Connection()
{
}

Connection::Connection(Connection const& connection)
:	m_socket(connection.m_socket)
{
}

// operators


// member functions
int	Connection::writeData()
{
	std::size_t	currentWrite = 0;

	currentWrite = write(m_socket, m_buffer + m_pos, m_size - m_pos);
	if (currentWrite < 0)
		return currentWrite;
	m_pos += currentWrite;
	if (m_pos == m_size)
	{
		m_pos = 0;
		m_size = 0;
	}
	return currentWrite;
}

int	Connection::readData()
{
	std::size_t	currentRead = 0;

	currentRead = read(m_socket, m_buffer, s_bufferSize);
	if (currentRead < 0)
		throw std::runtime_error(std::string(__func__) + ": read() error");
	m_pos += currentRead;
	return currentRead;
}
