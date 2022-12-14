#ifndef IIOMULTIPLEX_HPP
#define IIOMULTIPLEX_HPP

#include <vector>

template <typename Attr>
class	IIoMultiplex
{
protected:
	typedef typename Attr::EventData	EventData;
	typedef typename Attr::EventList	EventList;

	enum	e_flags
	{
		ADD = 1,
		DELETE = 2,
		MODIFY = 3 
	};

	enum	e_filters
	{
		READ = 0x1,
		WRITE = 0x2,
		ERROR = 0x4
	};

// constructors & destructor
	IIoMultiplex() {};
	virtual ~IIoMultiplex() {};

public:
// member functions
	virtual void				set(int fd, const EventData& event) = 0;
	virtual void				set(int fd, e_flags flag, e_filters filter) = 0;
	virtual const EventList&	poll() = 0;
};

#endif
