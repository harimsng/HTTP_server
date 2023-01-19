#include "PutMethod.hpp"

// constructors & destructor
PutMethod::PutMethod()
{
}

PutMethod::~PutMethod()
{
}

PutMethod::PutMethod(const PutMethod& putMethod)
{
	(void)putMethod;
}

// operators
PutMethod&
PutMethod::operator=(const PutMethod& putMethod)
{
	(void)putMethod;
	return *this;
}
