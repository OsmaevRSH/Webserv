#include "Server.hpp"
#include "Parser.hpp"
#include "MIME_ERROR.hpp"

int main(int argc, char **argv, char **env)
{
	MIME_ERROR mime;
#ifndef TESTER
	Parser config("./App/Config/conf");
#endif
#ifdef TESTER
	Parser config(argv[1]);
#endif
	Server server(config.getServers(), config.getErrorPages(), config.getEw(), mime,env);
	server.server_start();
	return 0;
}
