#ifndef IOMULTIPLEX_HPP
#define IOMULTIPLEX_HPP

#include <vector>

/*
struct	EventProperties
{
	enum	e_eventStatus
	{
		NORMAL = 1,
		END = 2,
		STAT_ERROR = 3,
		NON_BLOCK = 4
	};

	enum	e_operation
	{
		ADD = 1,
		DELETE = 2,
		MODIFY = 3,
		OP_ERROR = 4
	};

	enum	e_filters
	{
		NONE = 0x0,
		READ = 0x1,
		WRITE = 0x2,
		READWRITE = 0x3,
		FILT_ERROR = 0x4
	};
};
*/

template <typename T, typename Container = std::vector<T> >
// TODO: inheriting EventProperties is awkward
class	IoMultiplex//: public EventProperties
{
public:
	enum	e_eventStatus
	{
		NORMAL = 1,
		END = 2,
		STAT_ERROR = 3,
		NON_BLOCK = 4
	};

	enum	e_operation
	{
		ADD = 1,
		DELETE = 2,
		MODIFY = 3,
		OP_ERROR = 4
	};

	enum	e_filters
	{
		NONE = 0x0,
		READ = 0x1,
		WRITE = 0x2,
		READWRITE = 0x3,
		FILT_ERROR = 0x4
	};

	typedef T					Event;
	typedef Container			EventList;
	typedef enum e_eventStatus	EventStatus;

protected:
// constructors & destructor
	IoMultiplex() {};
	~IoMultiplex() {};

public:
// member functions
	void	add(int fd, const Event& event)
	{
		addWork(fd, event);
	}

	void	add(int fd, e_operation op, e_filters filter, void* userData)
	{
		addWork(fd, op, filter, userData);
	}

	const EventList&	poll()
	{
		return pollWork();
	}

private:
	virtual void	addWork(int fd, const Event& event) = 0;
	virtual void	addWork(int fd, e_operation op, e_filters filter, void* userData) = 0;
	virtual const EventList&	pollWork() = 0;

protected:
	EventList	m_changeList;
	EventList	m_eventList;
};

#endif
