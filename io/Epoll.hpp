#ifdef __linux__
#ifndef EPOLL_HPP
#define EPOLL_HPP

class	Epoll
{
public:
// constructors & destructor
	Epoll();
	~Epoll();
	Epoll(Epoll const& epoll);

// operators
	Epoll	&operator=(Epoll const& epoll);

// member functions
};

#endif
#endif
