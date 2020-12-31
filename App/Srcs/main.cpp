#include "Server.hpp"
#include "Config.hpp"

int main()
{
	Config config("./App/Test/conf");
	Server server(config.getServers(), config);
	server.server_start();
	return 0;
}
