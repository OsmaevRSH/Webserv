#include "../Server/Server.hpp"
#include "Config.hpp"

int main()
{
#ifndef TESTER
	Config config("./App/Test/conf");
#endif
#ifdef TESTER
	Config config("./App/Test/conf_for_tester");
#endif
	Server server(config.getServers(), config);
	server.server_start();
	return 0;
}
