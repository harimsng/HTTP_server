#include "ServerManager.hpp"
#include "ConfigParser.hpp"

int	main(int argc, char **argv, char **envp)
{
	ConfigParser	configParser;
	ServerManager	serverManager;

	if (argc != 2)
		return 1;
	(void)argv;
	(void)envp;
}
