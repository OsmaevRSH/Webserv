#include "Server.hpp"
#include "Config.hpp"
#include "MIME_ERROR.hpp"

int main()
{
	MIME_ERROR mime;
#ifndef TESTER
	Config config("./App/Config/conf");
#endif
#ifdef TESTER
	Config config("./App/Config/conf_for_tester");
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime);
	server.server_start();
	return 0;
}
