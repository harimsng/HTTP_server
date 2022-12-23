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

using namespace	std;

bool	parseArgument(int argc, char **argv);

int	main(int argc, char **argv, char **envp)
{
	ServerManager<IO_EVENT_POLLER>	serverManager;

	if (parseArgument(argc, argv) == false)
	{
		std::cout << "usage: webserv  [--log=option]  config_path\n";
		return 1;
	}
	serverManager.parse(argv[argc - 1]);
	serverManager.run();
	(void)argv;
	(void)envp;
}

bool	parseArgument(int argc, char **argv)
{
	if (argc == 1)
		return false;
	for (int i = 1; i < argc - 1; ++i)
	{
		string				arg(argv[i]);
		string::size_type	pos;

		if (!(arg[0] == '-' && arg[1] == '-'))
			return false;
		pos = arg.find('=');
		if (arg.substr(2, pos - 2) != "log")
			return false;
		arg = arg.substr(pos + 1, string::npos);
		Logger::initLogger(arg);
	}
	return true;
}

const string
getDate(const char* format)
{
	time_t		curTime;
	struct tm*	curTimeInfo;
	char		timeBuf[1024];

	time(&curTime);
	curTimeInfo = localtime(&curTime);
	strftime(timeBuf, 1024, format, curTimeInfo);
	return (timeBuf);
}
