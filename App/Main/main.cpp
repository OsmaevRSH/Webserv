#include "Server.hpp"
#include "Parser.hpp"
#include "MIME_ERROR.hpp"

int main(int argc, char **argv, char **env)
{
	MIME_ERROR mime;
#ifndef TESTER
	Config config("./App/Config/conf");
#endif
#ifdef TESTER
	Parser config("./App/Config/conf_for_tester");
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime,env);
	server.server_start();
	return 0;
}
