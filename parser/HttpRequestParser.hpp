#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "AParser.hpp"
#include "RequestTokenizer.hpp"
#include "Communicator.hpp"

class	HttpRequestParser : AParser<RequestTokenizer, Communicator::requestHeaderMap>
{
	typedef Communicator::requestHeaderMap requestHeaderMap ;
public:
// constructors & destructor
	HttpRequestParser();
	~HttpRequestParser();

// member functions
	virtual void	parse(requestHeaderMap& outputClass);
};

#endif
