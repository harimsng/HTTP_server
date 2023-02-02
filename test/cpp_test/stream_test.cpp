#include <iostream>
#include <sstream>
#include <string>

int	main()
{
	std::stringstream	ss;
	std::string			str = "    abc 123   ";

	ss.str(str);
	while (ss.good())
	{
		str.clear();
		ss >> str;
		if (str.size() == 0)
			continue;
		std::cout << str << '\n';
	}
}
