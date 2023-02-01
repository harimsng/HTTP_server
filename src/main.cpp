#include "Webserv.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#//include "parser/ConfigParser.hpp"
//#include "exception/ConfigParserException.hpp"
#include "util/Util.hpp"

using namespace	std;

// use char **environ for environment variables
int	main(int argc, char **argv)
{
	ServerManager	serverManager;

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
		LOG(ERROR, "%s", e.what());
		return 1;
	}
	return 0;
}
