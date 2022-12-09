#include <iostream>

#include "ServerManager.hpp"
#include "ConfigParser.hpp"

bool	parseArgument(int argc, char **argv);

int	main(int argc, char **argv, char **envp)
{
	ServerManager	serverManager;

	if (parseArgument(argc, argv) == false)
		return 1;
	serverManager.parse(argv[1]);
	serverManager.run();
	(void)argv;
	(void)envp;
}

bool	parseArgument(int argc, char **argv)
{
	(void)argv;
	if (argc != 2)
	{
		std::cout << "usage: webserv  config_path\n";
		return false;
	}
	return true;
}
