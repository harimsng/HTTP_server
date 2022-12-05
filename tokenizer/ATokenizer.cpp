#include "ATokenizer.hpp"

// constructors & destructor
ATokenizer::ATokenizer()
{
}

ATokenizer::~ATokenizer()
{
}

void	ATokenizer::eat(const std::string& target)
{
	if (get() != target)
	{
		// throw exception
	}
}
