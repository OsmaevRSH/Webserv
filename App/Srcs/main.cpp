#include "../Server/Server.hpp"
#include "Config.hpp"
#include "MIME.hpp"

int main()
{
	MIME mime;
#ifndef TESTER
	Config config("./App/Test/conf");
#endif
#ifdef TESTER
	Config config("./App/Test/conf_for_tester");
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime);
	server.server_start();
	return 0;
}
