#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

class	AutoIndex
{
// deleted
	AutoIndex	&operator=(const AutoIndex& autoIndex) {(void)autoIndex; return *this;}

public:
// constructors & destructor
	AutoIndex();
	~AutoIndex();
	AutoIndex(const AutoIndex& autoIndex);

// member functions
};

#endif
