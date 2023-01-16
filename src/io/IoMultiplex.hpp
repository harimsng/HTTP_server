#ifndef IOMULTIPLEX_HPP
#define IOMULTIPLEX_HPP

#include <vector>

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
};

#endif
