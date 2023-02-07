#include <iostream>
#include <string>
#include <sstream>

std::string
toHex(unsigned int num)
{
	std::stringstream	ss;

	ss.flags(std::ios_base::hex);
	ss << num;
	return ss.str();
#if 0
#endif
}

int	main()
{
	unsigned int	num = 16;

	std::cout << toHex(num) << '\n';
}
