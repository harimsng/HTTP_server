#ifndef IOMULTIPLEX_HPP
#define IOMULTIPLEX_HPP

#include <vector>

class	EventObject;

template <typename T>
class	IoMultiplex
{
public:
	enum	e_eventStatus
	{
		NORMAL = 1,
		END = 2,
		NON_BLOCK = 3,
		STAT_ERROR = 4,
	};

	enum	e_operation
	{
		ADD = 1,
		DELETE = 2,
		MODIFY = 3,
	};

	enum	e_filters
	{
		NONE = 0x0,
		READ = 0x1,
		WRITE = 0x2,
		READWRITE = 0x3,
		ERROR = 0x4
	};

	typedef T					Event;
	typedef enum e_eventStatus	EventStatus;
	typedef std::vector<T>		EventList;

protected:
// constructors & destructor
	IoMultiplex() {};
	~IoMultiplex() {};

public:
// member functions
	void	add(int fd, e_operation op, e_filters filter, EventObject* userData)
	{addWork(fd, op, filter, userData);}

	int		poll() {return pollWork();}

private:
	virtual void	addWork(int fd, e_operation op, e_filters filter, EventObject* userData) = 0;
	virtual int		pollWork() = 0;
};

#endif
