#ifndef IIOMULTIPLEX_HPP
#define IIOMULTIPLEX_HPP

struct	IoMultiplexEnum
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
		READ = 0x1,
		WRITE = 0x2,
		FILT_ERROR = 0x4
	};
};

template <typename Attr>
class	IIoMultiplex: public IoMultiplexEnum
{
public:
	typedef typename Attr::EventData	EventData;
	typedef typename Attr::EventList	EventList;
	typedef enum e_eventStatus			EventStatus;

protected:
// constructors & destructor
	IIoMultiplex() {};
	virtual ~IIoMultiplex() {};

public:
// member functions
	virtual void				add(int fd, const EventData& event) = 0;
	virtual void				add(int fd, e_operation flag, e_filters filter) = 0;
	virtual const EventList&	poll() = 0;
};

#endif
