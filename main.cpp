#include "ServerManager.hpp"
#include "ConfigParser.hpp"
#include "exception/ConfigParserException.hpp"

int	main(int argc, char **argv)
{
	ServerManager	serverManager;

	if (argc != 2)
		return 1;
	try {
	serverManager.run(argv[1]);
	} catch (ConfigParserException& e) {

	}
	return (0);
}
