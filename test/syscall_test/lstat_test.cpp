#include <sys/stat.h>
#include <iostream>
#include <string>

int	main()
{
	struct stat	status;
	std::string	str = "/Users/hseong/webserv/html/favicon-16x16.png";

	int	ret = lstat(str.c_str(), &status);

	std::cout << "ret = " << ret << '\n';
}
