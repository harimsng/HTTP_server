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

	char	front() const;
	char	back() const;

	void	pop_back();
};

#endif
