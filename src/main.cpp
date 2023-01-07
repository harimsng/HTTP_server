#ifdef __APPLE__
# define IO_EVENT_POLLER Kqueue
# define KQUEUE_IO
# include "io/Kqueue.hpp"
#elif __linux__
# define IO_EVENT_POLLER Epoll
# define EPOLL_IO
# include "io/Epoll.hpp"
#endif

#include <iostream>
#include <string>

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
	serverManager.parse(argv[argc - 1]);
	serverManager.run();
	(void)argv;
	(void)envp;
}
