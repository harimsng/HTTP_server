#include <iostream>
#include <string>

#include "Webserv.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include "parser/ConfigParser.hpp"
#include "exception/ConfigParserException.hpp"
#include "util/Util.hpp"

using namespace	std;

int	main(int argc, char **argv, char **envp)
{
	ServerManager<IO_EVENT_POLLER>	serverManager;

	if (Util::parseArgument(argc, argv) == false)
	{
		std::cout << "usage: webserv  [--log=option]  config_path\n";
		return 1;
	}
	try
	{
		serverManager.parseConfig(argv[argc - 1]);
		serverManager.run();
	}
	catch (std::exception& e)
	{
		cout << e.what();
		return 1;
	}
	catch (int status)
	{
		return status;
	}
	(void)argv;
	(void)envp;
	return 0;
}
